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

//testing
#include "subprocess.hpp"
#include "trace_parser.hpp"

using namespace vwatch;

/// @brief Main entry point.  Parse CLI args, daemonize, register 
///        signal handlers and then init vwatchd.
int main(int argc, char *argv[])
{
    // parse cli args
    settings::parse_cli_args(argc, argv);

    // daemonize (and close stdio)
    if (settings::runtime.daemon_mode)
        daemon::init();

    // register signal handlers
    signals::register_handlers();

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

    // allow other threads to execute (or run io_service?)
    while (true)
        sleep(100);

    // never reached
    exit(EXIT_SUCCESS);
}
