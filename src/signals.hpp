///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   POSIX signal handling functions
// @author  ben
// @date    11/1/2011
///////////////////////////////////////////////////////

#ifndef SIGNALS_HPP_VWATCHD
#define SIGNALS_HPP_VWATCHD

#include <pthread.h>

namespace vwatch {
    

    class signals {

    public:
        static void register_handlers();
        static void action_ignore(const int a_sig);
        static void action_exit(const int a_sig);
        static void action_child(const int a_sig);
        static void action_reload(const int a_sig);

    private:
        static sigset_t initial_sigmask;
        static sigset_t blocked_signals;
        static pthread_t handler_thread_id;

        static void *handler_thread(void *unused);
        static void dispatch_signal(int a_sig);
    };
    
}


#endif // incl guard
