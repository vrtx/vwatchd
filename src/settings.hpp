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

#ifndef VWATCHD_SETTINGS_HPP
#define VWATCHD_SETTINGS_HPP

namespace vwatch {

    // Runtime Settings Struct
    typedef struct {
        bool daemon_mode;
        bool debug_mode;
        bool syslog;
    } settings_t;

    // @class Runtime and dynamic configuration
    ///////////////////////////////////////////
    class settings {
    public:

        // Variables
        static settings_t runtime;    

        // Functions
        static void parse_cli_args(const int argc, char *argv[]);

    };

}

#endif // incl guard