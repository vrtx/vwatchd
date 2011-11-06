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

#include "common.hpp"
#include "version.hpp"
#include "signals.hpp"
#include "settings.hpp"
#include "daemon.hpp"

using namespace vwatch;

/// @brief Main entry point.  Parse CLI args, daemonize, register 
///        signal handlers and then init vwatchd.
int main(int argc, char *argv[])
{
    // parse cli args
    settings::parse_cli_args(argc, argv);

    // register signal handlers
    signals::register_handlers();
    
    // daemonize
    if (settings::runtime.daemon_mode)
        daemon::init();

    // ensure default mask so children can rwx at will
    umask(0);

    // change cwd to root
    if ((chdir("/")) < 0) {
        printf("Error:  Unable to change to root directory.  Reason: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // initialize syslog
    openlog("vwatchd", LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);
    if (settings::runtime.debug_mode) 
        setlogmask(LOG_UPTO(LOG_DEBUG));
    else
        setlogmask(LOG_UPTO(LOG_NOTICE));

    // print initialization message
    syslog(LOG_NOTICE, "Starting vwatchd version %s", VWATCHD_VERSION_STRING);
#ifdef VWATCH_STAP
    syslog(LOG_NOTICE, "vwatchd watcher: SystemTap");
#elif VWATCH_DTRACE
    syslog(LOG_NOTICE, "vwatchd watcher: DTrace");
#else
    #error "Watcher type not defined.  Set -DVWATCH_STAP for Linux systems or -DVWATCH_DTRACE for MacOS, Solaris or *BSD."
#endif

    // load config file
    // spawn subprocesses
    sleep(100);
    
    // spawn io service
    exit(EXIT_SUCCESS);
}
