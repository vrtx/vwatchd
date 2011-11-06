///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   Process spawning code
// @author  ben
// @date    11/3/2011
///////////////////////////////////////////////////////

#ifndef SUBPROCESS_HPP_VWATCHD
#define SUBPROCESS_HPP_VWATCHD

#include "common.hpp"

#define PIDFILE_PATH "/tmp/vwatchd.pid"

namespace vwatch {
    
    /// @class subprocess  Spawn a local or remote process for
    ///               data colection
    class subprocess {
    public:

        // default ctor
        subprocess();

        // dtor
        ~subprocess();

        // spawn a subprocess
        static subprocess *spawn(const string &path, const string &args, 
                                 string &out, string &out_err,
                                 int timeout, const size_t output_limit);

        // get output from a subprocess
        int get_output(string &out, string &err, int &ret);

    private:
        // instance members
        int out_pipe[2];    // pipe for child -> parent output
        int err_pipe[2];    // pipe for child -> parent error
        // helpers for pipe names
        int &out_read;
        int &out_write;
        int &err_read;
        int &err_write;
    };
    
}

#endif