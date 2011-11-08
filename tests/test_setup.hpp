//
// vwatchd test helper
//
// USAGE:  Include this file
//         Implement setup(), run() and teardown() methods
//         Add 'VWATCHD_RUN_TEST' to the end of the file
//
#ifndef VWATCHD_TEST_SETUP_HPP
#define VWATCHD_TEST_SETUP_HPP

#include "common.hpp"
#include "version.hpp"
#include "signals.hpp"
#include "settings.hpp"
#include "daemon.hpp"

using namespace vwatch;

#define VWATCHD_RUN_TEST                        \
int main(int argc, char *argv[])                \
{                                               \
    /* parse cli args */                        \
    settings::parse_cli_args(argc, argv);       \
                                                \
    /* redirect syslog to stderr */             \
    openlog("vwatchd", LOG_CONS | LOG_PERROR, LOG_DAEMON);  \
    setlogmask(LOG_UPTO(LOG_DEBUG));            \
                                                \
                                                \
    /* register signal handlers */              \
    signals::register_handlers();               \
                                                \
    /* setup, run test, teardown */             \
    try {                                       \
        setup();                                \
        runtest();                              \
        teardown();                             \
    } catch(...) {                              \
        cout << "ERROR: Exception Encountered." << endl; \
    }                                           \
    cout << endl << "DONE." << endl;            \
                                                \
    /* exit successfully after run */           \
    exit(EXIT_SUCCESS);                         \
                                                \
}                                               \

#endif