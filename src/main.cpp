///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   Initialize the vwatch daemon
// @author  ben
// @date    11/1/2011
///////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "version.hpp"
#include "settings.hpp"

using namespace vwatch;

/// @brief Main entry point.  Parse CLI args, daemonize, register 
///        signal handlers and then init vwatchd.
int main(int argc, char *argv[])
{
    pid_t pid;
    pid_t sid;

    settings::parse_cli_args(argc, argv);

    // fork the daemon process
    pid = fork();

    if (pid < 0) {
        // parent: fork failure
        printf("Error: Unable to fork child process.  Reason: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // become the session leader
    if ((sid = setsid()) == -1) {
        // fail
        printf("Error:  Unable to create process session group.  Reason: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // set default file mask
    umask(0);

    // change cwd
    if ((chdir("/")) < 0) {
        printf("Error:  Unable to change to root directory.  Reason: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // initialize syslog
    openlog("vwatchd", LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
    settings::runtime.debug_mode ? 
        setlogmask(LOG_UPTO(LOG_DEBUG)) : setlogmask(LOG_UPTO(LOG_NOTICE));

    // close stdio
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // print initialization message
    syslog(LOG_NOTICE, "Starting vwatchd version %s", VWATCHD_VERSION_STRING);
#ifdef VWATCH_STAP
    syslog(LOG_NOTICE, "vwatchd watcher: SystemTap");
#elif VWATCH_DTRACE
    syslog(LOG_NOTICE, "vwatchd watcher: DTrace");
#else
    #error "Watcher type not defined.  Set -DVWATCH_STAP for Linux systems or -DVWATCH_DTRACE for MacOS, Solaris or *BSD."
#endif

    exit(EXIT_SUCCESS);
}
