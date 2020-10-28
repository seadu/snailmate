#ifndef RBIMPL_INTERN_THREAD_H                       /*-*-C++-*-vi:se ft=cpp:*/
#define RBIMPL_INTERN_THREAD_H
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
 * @brief      Public APIs related to ::rb_cThread.
 */
#include "ruby/internal/attr/nonnull.h"
#include "ruby/internal/cast.h"
#include "ruby/internal/config.h"
#include "ruby/internal/dllexport.h"
#include "ruby/internal/value.h"

RBIMPL_SYMBOL_EXPORT_BEGIN()

struct timeval;

/* thread.c */

/**
 * Tries to switch  to another thread.  This function blocks  until the current
 * thread re-acquires the GVL.
 *
 * @exception  rb_eInterrupt  Operation interrupted.
 */
void rb_thread_schedule(void);

/**
 * Blocks the  current thread until  the given file  descriptor is ready  to be
 * read.
 *
 * @param[in]  fd                    A file descriptor.
 * @exception  rb_eIOError           Closed stream.
 * @exception  rb_eSystemCallError   Situations like EBADF.
 */
int rb_thread_wait_fd(int fd);

/**
 * Identical to rb_thread_wait_fd(), except it  blocks the cu