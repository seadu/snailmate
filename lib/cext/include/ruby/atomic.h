#ifndef RUBY_ATOMIC_H                                /*-*-C++-*-vi:se ft=cpp:*/
#define RUBY_ATOMIC_H
/**
 * @file
 * @author     Ruby developers <ruby-core@ruby-lang.org>
 * @copyright  This  file  is   a  part  of  the   programming  language  Ruby.
 *             Permission  is hereby  granted,  to  either redistribute  and/or
 *             modify this file, provided that  the conditions mentioned in the
 *             file COPYING are met.  Consult the file for details.
 * @warning    Symbols   prefixed  with   either  `RBIMPL`   or  `rbimpl`   are
 *             implementation details.   Don't take  them as canon.  They could
 *             rapidly appear then vanish.  The name (path) of this header file
 *             is also an  implementation detail.  Do not expect  it to persist
 *             at the place it is now.  Developers are free to move it anywhere
 *             anytime at will.
 * @note       To  ruby-core:  remember  that   this  header  can  be  possibly
 *             recursively included  from extension  libraries written  in C++.
 *             Do not  expect for  instance `__VA_ARGS__` is  always available.
 *             We assume C99  for ruby itself but we don't  assume languages of
 *             extension libraries.  They could be written in C++98.
 * @brief      Atomic operations
 *
 * Basically, if  we could assume  either C11 or  C++11, these macros  are just
 * redundant.  Sadly we cannot.  We have to do them ourselves.
 */

#include "ruby/internal/config.h"

#ifdef STDC_HEADERS
# include <stddef.h>            /* size_t */
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>         /* ssize_t */
#endif

#if RBIMPL_COMPILER_SINCE(MSVC, 13, 0, 0)
# pragma intrinsic(_InterlockedOr)
#elif defined(__sun) && defined(HAVE_ATOMIC_H)
# include <atomic.h>
#endif

#include "ruby/assert.h"
#include "ruby/backward/2/limits.h"
#include "ruby/internal/attr/artificial.h"
#include "ruby/internal/attr/noalias.h"
#include "ruby/internal/attr/nonnull.h"
#include "ruby/internal/compiler_since.h"
#include "ruby/internal/cast.h"
#include "ruby/internal/value.h"
#include "ruby/internal/static_assert.h"
#include "ruby/internal/stdbool.h"

/*
 * Asserts that  your environment supports  more than one atomic  types.  These
 * days systems tend to have such property  (C11 was a standard of decades ago,
 * right?) but we still support older ones.
 */
#if defined(__DOXYGEN__) || defined(HAVE_GCC_ATOMIC_BUILTINS) || defined(HAVE_GCC_SYNC_BUILTINS)
# define RUBY_ATOMIC_GENERIC_MACRO 1
#endif

/**
 * Type  that  is eligible  for  atomic  operations.   Depending on  your  host
 * platform you might have  more than one such type, but we  choose one of them
 * anyways.
 */
#if defined(__DOXYGEN__)
using rb_atomic_t = std::atomic<unsigned>;
#elif defined(HAVE_GCC_ATOMIC_BUILTINS)
typedef unsigned int rb_atomic_t;
#elif defined(HAVE_GCC_SYNC_BUILTINS)
typedef unsigned int rb_atomic_t;
#elif defined(_WIN32)
typedef LONG rb_atomic_t;
#elif defined(__sun) && defined(HAVE_ATOMIC_H)
typedef unsigned int rb_atomic_t;
#else
# error No atomic operation found
#endif

/**
 * Atomically replaces the  value pointed by `var` with the  result of addition
 * of `val` to the old value of `var`.
 *
 * @param   var  A variable of ::rb_atomic_t.
 * @param   val  Value to add.
 * @return  What was stored in `var` before the addition.
 * @post    `var` holds `var + val`.
 */
#define RUBY_ATOMIC_FETCH_ADD(var, val) rbimpl_atomic_fetch_add(&(var), (val))

/**
 * Atomically  replaces  the  value  pointed   by  `var`  with  the  result  of
 * subtraction of `val` to the old value of `var`.
 *
 * @param   var  A variable of ::rb_atomic_t.
 * @param   val  Value to subtract.
 * @return  What was stored in `var` before the subtraction.
 * @post    `var` holds `var - val`.
 */
#define RUBY_ATOMIC_FETCH_SUB(var, val) rbimpl_atomic_fetch_sub(&(var), (val))

/**
 * Atomically  replaces  the  value  pointed   by  `var`  with  the  result  of
 * bitwise OR between `val` and the old value of `var`.
 *
 * @param   var   A variable of ::rb_atomic_t.
 * @param   val   Value to mix.
 * @return  void
 * @post    `var` holds `var | val`.
 * @note    For portability, this macro can return void.
 */
#define RUBY_ATOMIC_OR(var, val) rbimpl_atomic_or(&(var), (val))

/**
 * Atomically replaces the value pointed by  `var` with `val`.  This is just an
 * assignment, but you can additionally know the previous value.
 *
 * @param   var   A variable of ::rb_atomic_t.
 * @param   val   Value to set.
 * @return  What was stored in `var` before the assignment.
 * @post    `var` holds `val`.
 */
#define RUBY_ATOMIC_EXCHANGE(var, val) rbimpl_atomic_exchange(&(var), (val))

/**
 * Atomic compare-and-swap.   This stores  `val` to  `var` if  and only  if the
 * assignment changes  the value of `var`  from `oldval` to `newval`.   You can
 * detect whether the assignment happened or not using the return value.
 *
 * @param   var        A variable of ::rb_atomic_t.
 * @param   oldval     Expected value of `var` before the assignment.
 * @param   newval     What you want to store at `var`.
 * @retval  oldval     Successful assignment (`var` is now `newval`).
 * @retval  otherwise  Something else is at `var`; not updated.
 */
#define RUBY_ATOMIC_CAS(var, oldval, newval) \
    rbimpl_atomic_cas(&(var), (oldval), (newval))

/**
 * Identical to #RUBY_ATOMIC_EXCHANGE, except for the return type.
 *
 * @param   var   A variable of ::rb_atomic_t.
 * @param   val   Value to set.
 * @return  void
 * @post    `var` holds `val`.
 */
#define RUBY_ATOMIC_SET(var, val) rbimpl_atomic_set(&(var), (val))

/**
 * Identical to #RUBY_ATOMIC_FETCH_ADD, except for the return type.
 *
 * @param   var  A variable of ::rb_atomic_t.
 * @param   val  Value to add.
 * @return  void
 * @post    `var` holds `var + val`.
 */
#define RUBY_ATOMIC_ADD(var, val) rbimpl_atomic_add(&(var), (val))

/**
 * Identical to #RUBY_ATOMIC_FETCH_SUB, except for the return type.
 *
 * @param   var  A variable of ::rb_atomic_t.
 * @param   val  Value to subtract.
 * @return  void
 * @post    `var` holds `var - val`.
 */
#define RUBY_ATOMIC_SUB(var, val) rbimpl_atomic_sub(&(var), (val))

/**
 * Atomically increments the value pointed by `var`.
 *
 * @param   var  A variable of ::rb_atomic_t.
 * @return  void
 * @post    `var` holds `var + 1`.
 */
#define RUBY_ATOMIC_INC(var) rbimpl_atomic_inc(&(var))

/**
 * Atomically decrements the value pointed by `var`.
 *
 * @param   var  A variable of ::rb_atomic_t.
 * @return  void
 * @post    `var` holds `var - 1`.
 */
#define RUBY_ATOMIC_DEC(var) rbimpl_atomic_dec(&(var))

/**
 * Identical to #RUBY_ATOMIC_INC,  except it expects its  argument is `size_t`.
 * There are cases where ::rb_atomic_t is  32bit while `size_t` is 64bit.  This
 * should be used for size related operations to support such platforms.
 *
 * @param   var  A variable of `size_t`.
 * @return  void
 * @post    `var` holds `var + 1`.
 */
#define RUBY_ATOMIC_SIZE_INC(var) rbimpl_atomic_size_inc(&(var))

/**
 * Identical to #RUBY_ATOMIC_DEC,  except it expects its  argument is `size_t`.
 * There are cases where ::rb_atomic_t is  32bit while `size_t` is 64bit.  This
 * should be used for size related operations to support such platforms.
 *
 * @param   var  A variable of `size_t`.
 * @return  void
 * @post    `var` holds `var - 1`.
 */
#define RUBY_ATOMIC_SIZE_DEC(var) rbimpl_atomic_size_dec(&(var))

/**
 * Identical  to #RUBY_ATOMIC_EXCHANGE,  except  it expects  its arguments  are
 * `size_t`.  There  are cases where  ::rb_atomic_t is 32bit while  `size_t` is
 * 64bit.  This  should be  used for  size related  operations to  support such
 * platforms.
 *
 * @param   var  A variable of `size_t`.
 * @param   val   Value to set.
 * @return  What was stored in `var` before the assignment.
 * @post    `var` holds `val`.
 */
#define RUBY_ATOMIC_SIZE_EXCHANGE(var, val) \
    rbimpl_atomic_size_exchange(&(var), (val))

/**
 * Identical to #RUBY_ATOMIC_CAS, except it expects its arguments are `size_t`.
 * There are cases where ::rb_atomic_t is 32bit while `size_t` is 64bit.  This
 * should be used for size related operations to support such platforms.
 *
 * @param   var        A variable of `size_t`.
 * @param   oldval     Expected value of `var` before the assignment.
 * @param   newval     What you want to store at `var`.
 * @retval  oldval     Successful assignment (`var` is now `newval`).
 * @retval  otherwise  Something else is at `var`; not updated.
 */
#define RUBY_ATOMIC_SIZE_CAS(var, oldval, newval) \
    rbimpl_atomic_size_cas(&(var), (oldval), (newval))

/**
 * Identical to #RUBY_ATOMIC_ADD, except it expects its arguments are `size_t`.
 * There are cases where ::rb_atomic_t is 32bit while `size_t` is 64bit.  This
 * should be used for size related operations to support such platforms.
 *
 * @param   var  A variable of `size_t`.
 * @param   val  Value to add.
 * @return  void
 * @post    `var` holds `var + val`.
 */
#define RUBY_ATOMIC_SIZE_ADD(var, val) rbimpl_atomic_size_add(&(var), (val))

/**
 * Identical to #RUBY_ATOMIC_SUB, except it expects its argu