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