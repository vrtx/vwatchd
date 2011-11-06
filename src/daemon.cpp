///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   Daemonizing code
// @author  ben
// @date    11/3/2011
///////////////////////////////////////////////////////

#include "common.hpp"
#include "daemon.hpp"

using namespace vwatch;

int daemon::pidfile = 0;

/// @brief start the process a daemon (fork, set sid/umask, close stdio)
void daemon::init() {

    pid_t pid;
    pid_t sid;
    char tmp_pid_str[20]; // up to a 64-bit int.

    if (getppid() == 1) {
        // already daemonized (e.g. by os init system)
        syslog(LOG_NOTICE, "vWatchd already initalized via external daemonizer.");
        return;
    }

    // fork the daemon process
    if ((pid = fork()) < 0) {
        // parent: fork failure
        printf("Error: Unable to fork child process.  Reason: %s\n", strerror(errno));
        close(pidfile);
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // parent: successfully forked; exit so child will be reparented.
        close(pidfile);
        exit(EXIT_SUCCESS);
    }
    // now in child process

    // create a pid file to prevent multiple daemons from running
    if ((pidfile = open(PIDFILE_PATH, O_RDWR|O_CREAT, 0600)) == -1 ) {
        // failed to create pid file
        syslog(LOG_WARNING, "Error: vWatchd appears to already be running, or was not "
                          "cleanly shutdown.  Reason: unable to create pid file. If"
                          "vWatchd is not actually running, remove %s and try again.",
                          PIDFILE_PATH);
        exit(EXIT_FAILURE);
    }

    // lock the pidfile to ensure exclusive access
    if (lockf(pidfile, F_TLOCK, 0) == -1)
    {
        // failed to lock the pidfile exclusively; likely another process running.
        syslog(LOG_WARNING, "Error: vWatchd appears to already be running, or was not "
                          "cleanly shutdown. Reason: unable to lock pid file. If "
                          "vWatchd is not running, remove %s and try again.", 
                          PIDFILE_PATH);
        exit(EXIT_FAILURE);
    }

    // write out the pidfile, leaving it open until the daemon exits.
    sprintf(tmp_pid_str,"%d\n",getpid());
    write(pidfile, tmp_pid_str, strlen(tmp_pid_str));

    // create session for child process
    if ((sid = setsid()) == -1) {
        // fail
        printf("Error:  Unable to create process session group.  Reason: %s\n", strerror(errno));
        close(pidfile);
        exit(EXIT_FAILURE);
    }

    // close stdio
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
}

/// @brief cleanup after the daemon
void daemon::shutdown() 
{
    close(pidfile);
}    
    