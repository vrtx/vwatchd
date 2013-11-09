#include "common.hpp"
#include "signals.hpp"
#include "daemon.hpp"

using namespace vwatch;

// initialize statics
sigset_t signals::initial_sigmask;
sigset_t signals::blocked_signals;
pthread_t signals::handler_thread_id;

// initialize signal handler fn table
sig_handler_fn signals::sig_handler_table[VW_SIGMAX] = { signals::action_unhandled };

// register signal handlers and block any that may interfere
// with the daemon process.
void signals::register_handlers() {
  // set up the mask of signals to handle
  sigemptyset(&initial_sigmask);
  sigemptyset(&blocked_signals);
  sigaddset(&blocked_signals, SIGTTOU);  // handle stdout writes while daemonized
  sigaddset(&blocked_signals, SIGTTIN);  // handle stdin reads while daemonized
  sigaddset(&blocked_signals, SIGHUP);   // handle tty hangup
  sigaddset(&blocked_signals, SIGTSTP);  // handle C-z
  sigaddset(&blocked_signals, SIGCHLD);  // handle sigchild
  sigaddset(&blocked_signals, SIGINT);
  sigaddset(&blocked_signals, SIGQUIT);
  sigaddset(&blocked_signals, SIGUSR1);
  sigaddset(&blocked_signals, SIGUSR2);
  pthread_sigmask(SIG_BLOCK, &blocked_signals, &initial_sigmask);

  // add each signal handler to the fn ptr table
  sig_handler_table[SIGHUP] = action_reload;
  sig_handler_table[SIGTERM] = action_exit;
  sig_handler_table[SIGQUIT] = action_exit;
  sig_handler_table[SIGINT] = action_exit;
  sig_handler_table[SIGUSR1] = action_ignore;
  sig_handler_table[SIGUSR2] = action_ignore;
  sig_handler_table[SIGCHLD] = action_child;

  // spawn the signal handling thread
  if (pthread_create(&handler_thread_id, NULL, handler_thread, NULL) != 0) {
    // error
    syslog(LOG_CRIT, "Error: Unable to create thread to handle posix signals.  Reason: %s\n", strerror(errno));
  }
}

// ignore a signal explicitly 
void signals::action_ignore(int a_sig) {
  syslog(LOG_NOTICE, "Ignoring signal: %s.\n", strsignal(a_sig));
}

// handle request to ext the vwatchd process
void signals::action_exit(int a_sig) {
  syslog(LOG_NOTICE, "vWatchd exiting due to %s (%d).\n", strsignal(a_sig), a_sig);
  daemon::shutdown();
  exit(EXIT_SUCCESS);
}

// handle a sigchld signal from a child
void signals::action_child(int a_sig) {
  // wait for process to finish
  syslog(LOG_NOTICE, "Child process '%s' has exited %d.\n", "Name", a_sig);
  wait(NULL);
}

// signal handler to reload the process
void signals::action_reload(int a_sig) {
  syslog(LOG_NOTICE, "Terminating child process X. \n");
  syslog(LOG_NOTICE, "Closing Network connections. \n");
  syslog(LOG_NOTICE, "Closing Pipes. \n");
  syslog(LOG_NOTICE, "Syncing Files. \n");
}

// don't do anything with a signal
void signals::action_unhandled(int a_sig) {
  syslog(LOG_DEBUG, "Unhandled signal: %s.\n", strsignal(a_sig));
}

// signal handler thread
void *signals::handler_thread(void *unused) {
  int sig = 0;        // signal delivered from OS
  int fail_count = 0; // number of failed attempts to wait

  // wait for a signal
  while (true) {
    if (sigwait (&blocked_signals, &sig) != 0) {
      // waiting for a signal failed.  retry in 1 second
      syslog(LOG_CRIT, "Error: Unable to handle signal in handler thread.  Reason: %s", 
                        strerror(errno));
      if (++fail_count > 5) {
          syslog(LOG_CRIT, "Error: Too many failed attempts to wait for signals.  Reason: %s", 
                           strerror(errno));
          exit(EXIT_FAILURE);
      }
      sleep(1);
      continue; // retry
    }
    if (sig > VW_SIGMAX) {
      syslog(LOG_CRIT, "Error: Received invalid signal (%d), which is greater "
                       "than the max (%d).  Exiting.", sig, VW_SIGMAX);
      exit(EXIT_FAILURE);
    }
    syslog(LOG_DEBUG, "Received signal: %s (with max signal id = %d)\n", 
                      strsignal(sig), VW_SIGMAX);
    // call the appropriate function
    sig_handler_table[sig](sig);
  }
  // never reached
  return NULL;
}

// signal handler thread
void signals::unregister_handlers() {
  int err = 0;

  // kill the handler thread
  if ((err = pthread_cancel(handler_thread_id)) != 0) {
      // error
      syslog(LOG_CRIT, "Failed to cancel handler thread.  Reason: %s \n", 
                        strerror(err));
      return;
  }

  if ((err = pthread_join(handler_thread_id, NULL)) != 0) {
      syslog(LOG_WARNING, "Couldn't join to thread pending cancellation.  Reason: %s \n", 
                        strerror(err));
      return;
  }

  // restore signal mask
  pthread_sigmask(SIG_SETMASK, &initial_sigmask, NULL);
}

