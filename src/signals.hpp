///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   POSIX signal handling functions
  ///////////////////////////////////////////////////////

#pragma once

#include <pthread.h>
#include <signal.h>

// SIGRTMAX not available on all platforms (e.g. BSD)
#ifdef SIGRTMAX
#  define VW_SIGMAX SIGRTMAX
#else
#  define VW_SIGMAX SIGUSR2 + 8
#endif

namespace vwatch {

    // typedef for signal handing function pointers
  typedef void (*sig_handler_fn)(const int a_sig);

  class signals {
  public:
    static sig_handler_fn sig_handler_table[VW_SIGMAX];

    static void register_handlers();
    static void action_ignore(const int a_sig);
    static void action_exit(const int a_sig);
    static void action_child(const int a_sig);
    static void action_reload(const int a_sig);
    static void action_unhandled(const int a_sig);
    static void unregister_handlers();

  private:
    static sigset_t initial_sigmask;
    static sigset_t blocked_signals;
    static pthread_t handler_thread_id;

    static void *handler_thread(void *unused);
  };

}

