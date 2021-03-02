#ifndef RUBY_FIBER_SCHEDULER_H                       /*-*-C++-*-vi:se ft=cpp:*/
#define RUBY_FIBER_SCHEDULER_H
/**
 * @file
 * @author     Ruby developers <ruby-core@ruby-lang.org>
 * @copyright  This  file  is   a  part  of  the   programming  language  Ruby.
 *             Permission  is hereby  granted,  to  either redistribute  and/or
 *             modify this file, provided that  the conditions mentioned in the
 *             file COPYING are met.  Consult the file for details.
 * @brief      Scheduler APIs.
 */
#include "ruby/internal/config.h"

#include <errno.h>

#ifdef STDC_HEADERS
#include <stddef.h> /* size_t */
#endif

#include "ruby/ruby.h"
#include "ruby/internal/dllexport.h"
#include "ruby/internal/arithmetic.h"

RBIMPL_SYMBOL_EXPORT_BEGIN()

struct timeval;

/**
 * Wrap a `ssize_t` and `int errno` into a single `VALUE`. This interface should
 * be used to safely capture results from system calls  like `read` and `write`.
 *
 * You should use `rb_fiber_scheduler_io_result_apply` to unpack the result of
 * this value and update `int errno`.
 *
 * You should not directly try to interpret the result value as it is considered
 * an opaque representation. However, the general representation is an integer
 * in the range of `[-int errno, size_t size]`. Linux generally restricts the
 * result of system calls like `read` and `write` to `<= 2^31` which means this
 * will typically fit within a single FIXNUM.
 *
 * @param[in]  result   The result of the system call.
 * @param[in]  error    The value of `errno`.
 * @return              A `VALUE` which contains the result and/or errno.
 */
static inline VALUE
rb_fiber_scheduler_io_result(ssize_t result, int error) {
    if (result == -1) {
        return RB_INT2NUM(-error);
    } else {
        return RB_SIZE2NUM(result);
    }
}

/**
 * Apply an io result to the local thread, returning the value of the original
 * system call that created it and updating `int errno`.
 *
 * You should not directly try to interpret the result value as it is considered
 * an opaque representation.
 *
 * @param[in]  result   The `VALUE` which contains an errno and/or result size.
 * @post                Updates `int errno` with the value if negative.
 * @return              The original result of the system call.
 */
static inline ssize_t
rb_fiber_scheduler_io_result_apply(VALUE result) {
    if (RB_FIXNUM_P(result) && RB_NUM2INT(result) < 0) {
        errno = -RB_NUM2INT(result);
        return -1;
    } else {
        return RB_NUM2SIZE(result);
    }
}

/**
 * Queries the  current scheduler of  the current  thread that is  calling this
 * function.
 *
 * @retval  RUBY_Qnil  No scheduler has  been set so far to  this thread (which
 *                     is the default).
 * @retval  otherwise  The scheduler that  was last set for  the current thread
 *                     with rb_fiber_scheduler_set().
 */
VALUE rb_fiber_scheduler_get(void);

/**
 * Destructively assigns  the passed  scheduler to that  of the  current thread
 * that is calling this function.  If the scheduler is set, non-blocking fibers
 * (created by `Fiber.new` with `blocking: false`, or by `Fiber.schedule`) call
 * that scheduler's  hook methods on  potentially blocking operations,  and the
 * current  thread  will  call  scheduler's  `#close`  method  on  finalisation
 * (allowing  the  scheduler  to  properly  manage  all  non-finished  fibers).
 * `scheduler`   can   be   an   object   of   any   class   corresponding   to
 * `Fiber::SchedulerInterface`. Its implementation is up to the user.
 *
 * @param[in]  scheduler     The scheduler to set.
 * @exception  rb_eArgError  `scheduler` does not conform the interface.
 * @post       Current thread's scheduler is `scheduler`.
 */
VALUE rb_fiber_scheduler_set(VALUE scheduler);

/**
 * Identical to rb_fiber_scheduler_get(), except it also returns ::RUBY_Qnil in
 * case of a blocking fiber.  As blocking fibers do not participate schedulers'
 * scheduling this function can be handy.
 *
 * @retval  RUBY_Qnil  No scheduler is in effect.
 * @retval  otherwise  The scheduler that is in effect, if any.
 */
VALUE rb_fiber_scheduler_current(void);

/**
 * Identical to rb_fiber_scheduler_current(), except it queries for that of the
 * passed thread instead of the implicit current one.
 *
 * @param[in]  thread         Target thread.
 * @exception  rb_eTypeError  `thread` is not a thread.
 * @retval     RUBY_Qnil      No scheduler is in effect in `thread`.
 * @retval     otherwise      The scheduler that is in effect in `thread`.
 */
V