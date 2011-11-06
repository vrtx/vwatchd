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

#include <signal.h>

#include "common.hpp"
#include "signals.hpp"
#include "daemon.hpp"

using namespace vwatch;

// initialize statics
sigset_t signals::initial_sigmask;
sigset_t signals::blocked_signals;
pthread_t signals::handler_thread_id;

/// @brief register signal handlers and block any that may interfere
///        with the daemon process.
void signals::register_handlers()
{

    // block signals a daemon shouldn't handle
    sigemptyset(&initial_sigmask);
    sigemptyset(&blocked_signals);
    sigaddset(&blocked_signals, SIGTSTP);
    sigaddset(&blocked_signals, SIGTTOU);
    sigaddset(&blocked_signals, SIGTTIN);
    pthread_sigmask(SIG_BLOCK, &blocked_signals, &initial_sigmask);

    // spawn the signal handling thread
    if (pthread_create(&handler_thread_id, NULL, handler_thread, NULL) != 0) {
        // error
        printf("Error: Unable to create thread to handle posix signals.  Reason: %s", strerror(errno));
    }

    // TODO: LEFT OFF HERE

    // struct sigaction signal_handler;

    // register the dispatch function
    // signal_handler.sa_handler = dispatch_signal;
    // sigemptyset(&signal_handler.sa_mask);
    // signal_handler.sa_flags = 0;

    // // add handlers for the following signals:
    // sigaction(SIGHUP, &signal_handler, NULL); 
    // sigaction(SIGTERM, &signal_handler, NULL);
    // // sigaction(SIGINT, &signal_handler, NULL); 
    // sigaction(SIGUSR1, &signal_handler, NULL);
    // sigaction(SIGUSR2, &signal_handler, NULL);
    // sigaction(SIGCHLD, &signal_handler, NULL);
}

/// @brief Dispatch a POSIX signal to the handler function
void signals::dispatch_signal(const int a_sig)
{
    switch(a_sig) {
        case SIGHUP:
            syslog(LOG_NOTICE, "Received SIGHUP. \n", a_sig);
            action_reload(a_sig);
            break;
        case SIGTERM:  // fallthrough
        case SIGINT:
            action_exit(a_sig);
            break;
        case SIGUSR1:  // fallthrough
        case SIGUSR2:
            action_ignore(a_sig);
            break;
        case SIGCHLD:
            action_child(a_sig);
            break;
        default:
            syslog(LOG_WARNING, "Unkown signal (%d) received.", a_sig);
            break;
    }    
}

// 
void signals::action_ignore(int a_sig)
{
    syslog(LOG_NOTICE, "Ignoring signal %d.\n", a_sig);
    
}

/// @brief handle request to ext the vwatchd process
void signals::action_exit(int a_sig)
{
    syslog(LOG_NOTICE, "vWatchd exiting due to signal %d.\n", a_sig);
    daemon::shutdown();
}

/// @brief handle a sigchld signal from a child
void signals::action_child(int a_sig)
{
    syslog(LOG_NOTICE, "Child process '%s' has exited %d.\n", "Name", a_sig);
}

/// @brief signal handler to reload the process
void signals::action_reload(int a_sig)
{
    syslog(LOG_NOTICE, "Terminating child process X. \n", a_sig);
    syslog(LOG_NOTICE, "Closing Network connections. \n", a_sig);
    syslog(LOG_NOTICE, "Closing Pipes. \n", a_sig);
    syslog(LOG_NOTICE, "Syncing Files. \n", a_sig);
}

/// @brief signal handler thread
void *signals::handler_thread(void *unused)
{
    int sig;    // signal delivered from OS

    // wait for a signal
    while (true) {
        if (sigwait (&blocked_signals, &sig) != 0)
            // waiting for a signal failed
            syslog(LOG_CRIT, "Error: Unable to handle signal in handler thread.  Reason: %s", strerror(errno));

        printf("Got signal.\n");
        // call the appropriate function
        dispatch_signal(sig);
    }
    
    return NULL;
}
