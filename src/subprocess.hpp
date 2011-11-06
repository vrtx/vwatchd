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

#define PIDFILE_PATH "/tmp/vwatchd.pid"

namespace vwatch {
    
    /// @class subprocess  Spawn a local or remote process for
    ///               data colection
    class subprocess {
    public:

        static subprocess spawn(const string &path, const string &args, 
                                string &out, string &out_err,
                                int timeout, const size_t output_limit);

        int get_output(string &out, string &err);

    private:
        // instance members
        int out_pipe[2];    // pipe for child -> parent output
        int err_pipe[2];    // pipe for child -> parent error

        // helpers for pipe names
        const out_read = out_pipe[1];
        const out_write = out_pipe[2];
        const err_read = err_pipe[1];
        const err_write = err_pipe[2];
    };
    
}

#endif