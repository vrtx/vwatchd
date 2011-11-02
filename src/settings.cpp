///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   Runtime and dynamic configuration class
// @author  ben
// @date    11/1/2011
///////////////////////////////////////////////////////

#include <getopt.h>
#include "settings.hpp"

using namespace vwatch;

// initialize global runtime settings
settings_t settings::runtime = {
    true,   // daemon mode
    false,  // debug mode
    true,   // syslog
};

/// @brief Parse command line arguments and populate
///        global runtime_settings.
void settings::parse_cli_args(const int argc, char *argv[])
{
    int cli_arg = 0;
    while ((cli_arg = getopt(argc, argv, "fdvh:")) != -1) {
        // for each command line arg
        switch (cli_arg) {
        case 'f':
            // run in foreground
            settings::runtime.daemon_mode = false;
            break;
        case 'd':
            // run in foreground
            settings::runtime.debug_mode = true;
            break;
        case 'v':
            // display version and exit
            break;
        case 'h':
            // display help and exit
            break;
        }
    }
}

