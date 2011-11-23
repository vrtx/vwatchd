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

FILE *daemon::pidfile = 0;

/// @brief start the process a daemon (fork, set sid/umask, close stdio)
void daemon::init() {

    pid_t pid;
    pid_t sid;

    if (getppid() == 1) {
        // already daemonized (e.g. by os init system)
        syslog(LOG_NOTICE, "vWatchd already initalized via external daemonizer.");
        return;
    }

    // fork the daemon process
    if ((pid = fork()) < 0) {
        // parent: fork failure
        cerr << "Error: Unable to fork child process.  Reason: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // parent: successfully forked; exit so child will be reparented.
        exit(EXIT_SUCCESS);
    }
    // now in child process

    // create a pid file to prevent multiple daemons from running
    if ((pidfile = fopen(PIDFILE_PATH, "w+")) == NULL ) {
        // failed to create pid file
        cerr << "Unable to start vwatchd:  Failed to to create pid file.  Ensure"
                "vwatchd is not already running and remove " <<  PIDFILE_PATH << "." << endl;
        exit(EXIT_FAILURE); // exit without removing existing pidfile
    }

    // lock the pidfile to ensure exclusive access
    if (flock(fileno(pidfile), LOCK_EX | LOCK_NB) == -1) {
        // failed to lock the pidfile exclusively; likely another process running.
        cerr << "Unable to start vwatchd: vwatchd is already running or did not "
                "shutdown gracefully.  Ensure vwatchd is not running, remove "
                << PIDFILE_PATH << " and try again." << endl;
        exit(EXIT_FAILURE); // exit without removing existing pidfile
    }

    // write out the pidfile, leaving it open until the daemon exits.
    fprintf(pidfile,"%d",getpid());

    // create session for child process
    if ((sid = setsid()) == -1) {
        // fail
        cerr << "Error:  Unable to create process session group.  Reason: " << strerror(errno) << endl;
        goto clean_error;
    }

    // propagate file creation mask to children
    umask(0);

    // change cwd to root
    if ((chdir("/")) < 0) {
        cerr << "Error:  Unable to change to root directory.  Reason: " << strerror(errno) << endl;
        goto clean_error;
    }

    // close stdio
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    return;

clean_error:
    shutdown();
    exit(EXIT_FAILURE);
    return; // not reached

}

/// @brief cleanup after the daemon
void daemon::shutdown() 
{
    // close and delete the pidfile
    if (pidfile > 0)
        fclose(pidfile);
    unlink(PIDFILE_PATH);
}    


