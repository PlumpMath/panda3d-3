// Filename: threadSimpleImpl.h
// Created by:  drose (18Jun07)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#ifndef THREADSIMPLEIMPL_H
#define THREADSIMPLEIMPL_H

#include "pandabase.h"
#include "selectThreadImpl.h"

#ifdef THREAD_SIMPLE_IMPL

#include "pnotify.h"
#include "threadPriority.h"
#include "pvector.h"
#include "contextSwitch.h"

#ifdef HAVE_PYTHON

#undef _POSIX_C_SOURCE
#include <Python.h>

#endif  // HAVE_PYTHON

class Thread;
class ThreadSimpleManager;
class MutexSimpleImpl;

////////////////////////////////////////////////////////////////////
//       Class : ThreadSimpleImpl
// Description : This is a trivial threading implementation for
//               applications that don't desire full OS-managed
//               threading.  It is a user-space implementation of
//               threads implemented via setjmp/longjmp, and therefore
//               it cannot take advantage of multiple CPU's (the
//               application will always run on a single CPU,
//               regardless of the number of threads you spawn).
//
//               However, since context switching is entirely
//               cooperative, synchronization primitives like mutexes
//               and condition variables aren't necessary, and the
//               Mutex and ConditionVar classes are compiled into
//               trivial no-op classes, which can reduce overhead
//               substantially compared to a truly threaded
//               application.
//
//               Be sure that every thread calls
//               Thread::consider_yield() occasionally, or it will
//               starve the rest of the running threads.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_PIPELINE ThreadSimpleImpl {
public:
  ThreadSimpleImpl(Thread *parent_obj);
  ~ThreadSimpleImpl();

  void setup_main_thread();
  bool start(ThreadPriority priority, bool joinable);
  void join();
  void preempt();

  string get_unique_id() const;

  static void prepare_for_exit();

  INLINE static Thread *get_current_thread();
  INLINE static void bind_thread(Thread *thread);
  INLINE static bool is_threading_supported();
  INLINE static bool is_true_threads();
  INLINE static void sleep(double seconds);
  INLINE static void yield();
  INLINE static void consider_yield();

  void sleep_this(double seconds);
  void yield_this(bool volunteer);
  INLINE void consider_yield_this();

  INLINE double get_wake_time() const;

  INLINE static void write_status(ostream &out);

private:
  static void st_begin_thread(void *data);
  void begin_thread();

private:
  enum Status {
    S_new,
    S_running,
    S_finished,
    S_killed,
  };

  static int _next_unique_id;
  int _unique_id;
  Thread *_parent_obj;
  bool _joinable;
  Status _status;

  // The relative weight of this thread, relative to other threads, in
  // priority.
  double _priority_weight;

  // The amount of time this thread has run recently.
  unsigned int _run_ticks;

  // This is the time at which the currently-running thread started
  // execution.
  double _start_time;

  // This is the time at which the currently-running thread should
  // yield.
  double _stop_time;

  // This records the time at which a sleeping thread should wake up.
  double _wake_time;

  ThreadContext _context;
  unsigned char *_stack;
  size_t _stack_size;

#ifdef HAVE_PYTHON
  // If we might be working with Python, we have to manage the Python
  // thread state as we switch contexts.
  PyThreadState *_python_state;
#endif  // HAVE_PYTHON

  // Threads that are waiting for this thread to finish.
  typedef pvector<ThreadSimpleImpl *> JoiningThreads;
  JoiningThreads _joining_threads;

  ThreadSimpleManager *_manager;
  static ThreadSimpleImpl *volatile _st_this;

  friend class ThreadSimpleManager;
};

// We include this down here to avoid the circularity problem.
/* okcircular */
#include "threadSimpleManager.h"

#include "threadSimpleImpl.I"

#endif // THREAD_SIMPLE_IMPL

#endif
