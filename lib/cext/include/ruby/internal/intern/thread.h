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
 * Identical to rb_thread_wait_fd(), except it  blocks the current thread until
 * the given file descriptor is ready to be written.
 *
 * @param[in]  fd                    A file descriptor.
 * @exception  rb_eIOError           Closed stream.
 * @exception  rb_eSystemCallError   Situations like EBADF.
 */
int rb_thread_fd_writable(int fd);

/**
 * Notifies a closing of a file  descriptor to other threads.  Multiple threads
 * can wait for the given file descriptor  at once.  If such file descriptor is
 * closed, threads need to start propagating their exceptions.  This is the API
 * to kick that process.
 *
 * @param[in]  fd  A file descriptor.
 * @note       This function blocks  until all the threads waiting  for such fd
 *             have woken up.
 */
void rb_thread_fd_close(int fd);

/**
 * Checks if  the thread this  function is running is  the only thread  that is
 * currently alive.
 *
 * @retval  1  Yes it is.
 * @retval  0  No it isn't.
 *
 * @internal
 *
 * Above description is in fact inaccurate.  There are Ractors these days.
 */
int rb_thread_alone(void);

/**
 * Blocks for the given period of time.
 *
 * @warning    This function can be interrupted by signals.
 * @param[in]  sec            Duration in seconds.
 * @exception  rb_eInterrupt  Interrupted.
 */
void rb_thread_sleep(int sec);

/**
 * Blocks indefinitely.
 *
 * @exception  rb_eInterrupt  Interrupted.
 */
void rb_thread_sleep_forever(void);

/**
 * Identical  to  rb_thread_sleep_forever(),  except the  thread  calling  this
 * function is considered "dead" when our deadlock checker is triggered.
 *
 * @exception  rb_eInterrupt  Interrupted.
 */
void rb_thread_sleep_deadly(void);

/**
 * Stops the current thread.  This is not the end of the thread's lifecycle.  A
 * stopped thread can later be woken up.
 *
 * @exception  rb_eThreadError  Stopping this thread would deadlock.
 * @retval     ::RUBY_Qnil      Always.
 *
 * @internal
 *
 * The return value makes no sense at all.
 */
VALUE rb_thread_stop(void);

/**
 * Marks a given thread as eligible for scheduling.
 *
 * @note  It may still remain blocked on I/O.
 * @note  This does not invoke the scheduler itself.
 *
 * @param[out]  thread           Thread in question to wake up.
 * @exception   rb_eThreadError  Stop flogging a dead horse.
 * @return      The passed thread.
 * @post        The passed thread is made runnable.
 */
VALUE rb_thread_wakeup(VALUE thread);

/**
 * Identical  to rb_thread_wakeup(),  except  it doesn't  raise  on an  already
 * killed thread.
 *
 * @param[out]  thread     A thread to wake up.
 * @retval      RUBY_Qnil  `thread` is already killed.
 * @retval      otherwise  `thread` is alive.
 * @post        The passed thread is made runnable, unless killed.
 */
VALUE rb_thread_wakeup_alive(VALUE thread);

/**
 * This is a rb_thread_wakeup() + rb_thread_schedule() combo.
 *
 * @note        There is no  guarantee that this function yields  to the passed
 *              thread.  It may still remain blocked on I/O.
 * @param[out]  thread           Thread in question to wake up.
 * @exception   rb_eThreadError  Stop flogging a dead horse.
 * @return      The passed thread.
 */
VALUE rb_thread_run(VALUE thread);

/**
 * Terminates the given thread.  Unlike a stopped thread, a killed thread could
 * never be revived.   This function does return, when passed  e.g.  an already
 * killed thread.   But if  the passed  thread is  the only  one, or  a special
 * thread called "main", then it also terminates the entire process.
 *
 * @param[out]  thread          The thread to terminate.
 * @exception   rb_eFatal       The passed thread is the running thread.
 * @exception   rb_eSystemExit  The passed thread is the last thread.
 * @return      The passed thread.
 * @post        Either the passed thread, or the process entirely, is killed.
 *
 * @internal
 *
 * It seems killing the main thread also kills the entire process even if there
 * are multiple running ractors.  No idea why.
 */
VALUE rb_thread_kill(VALUE thread);

RBIMPL_ATTR_NONNULL((1))
/**
 * Creates a Ruby thread that is backended by a C function.
 *
 * @param[in]      f                    The function to run on a thread.
 * @param[in,out]  g                    Passed through to `f`.
 * @exception      rb_eThreadError      Could not create a ruby thread.
 * @exception      rb_eSystemCallError  Situations like `EPERM`.
 * @return         Allocated instance of ::rb_cThread.
 * @note           This doesn't wait for anything.
 */
VALUE rb_thread_create(VALUE (*f)(void *g), void *g);

/**
 * Identical to rb_thread_sleep(), except it takes struct `timeval` instead.
 *
 * @warning    This function can be interrupted by signals.
 * @param[in]  time           Duration.
 * @exception  rb_eInterrupt  Interrupted.
 */
void rb_thread_wait_for(struct timeval time);

/**
 * Obtains the "current" thread.
 *
 * @return  The current thread  of the current ractor of  the current execution
 *          context.
 * @pre     This function must be called from a thread controlled by ruby.
 */
VALUE rb_thread_current(void);

/**
 * Obtains the "main" thread.  There are threads called main.  Historically the
 * (only) main thread was the one which  runs when the process boots.  Now that
 * we have Ractor, there are more than one main threads.
 *
 * @return  The  main thread  of the  current ractor  of the  current execution
 *          context.
 * @pre     This function must be called from a thread controlled by ruby.
 */
VALUE rb_thread_main(void);

/**
 * This  badly named  function reads  from a  Fiber local  storage.  When  this
 * function was  born there  was no  such thing  like a  Fiber.  The  world was
 * innocent.  But now...  This is a Fiber local storage.  Sorry.
 *
 * @param[in]  thread     Thread that the target Fiber is running.
 * @param[in]  key        The name of the Fiber local storage to read.
 * @retval     RUBY_Qnil  No such storage.
 * @retval     otherwise  The value stored at `key`.
 * @note       There in fact are "true"  thread local storage, but Ruby doesn't
 *             provide any interface of them to you, C programmers.
 */
VALUE rb_thread_local_aref(VALUE thread, ID key);

/**
 * This  badly named  function  writes to  a Fiber  local  storage.  When  this
 * function was  born there  was no  such thing  like a  Fiber.  The  world was
 * innocent.  But now...  This is a Fiber local storage.  Sorry.
 *
 * @param[in]  thread           Thread that the target Fiber is running.
 * @param[in]  key              The name of the Fiber local storage to write.
 * @param[in]  val              The new value of the storage.
 * @exception  rb_eFrozenError  `thread` is frozen.
 * @return     The passed `val` as-is.
 * @post       Fiber local storage `key` has value of `val`.
 * @no