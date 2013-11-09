///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// Code to handle spawning and managing a subprocess
///////////////////////////////////////////////////////

#pragma once

#include "common.hpp"

namespace vwatch {

  class subproc_stdio;

  /// @class subprocess  Spawn a local or remote process for
  ///               data colection
  class subprocess {
  public:
    string path_;      // path to binary
    string args_;      // arguments to pass to executable
    int retcode_;      // return code of the child

    // default ctor
    subprocess();
    ~subprocess();

    // spawn a subprocess
    static subprocess *spawn(const string &path, subproc_stdio &stdio,
                             const string args = "", const string cwd = "/",
                             int timeout = 0, const size_t output_limit = 0);

    // get output from a subprocess that has finished running
    int get_complete_output(string &out, string &err, int &retcode) const;

    // print the status of the subprocess
    string get_status() const;

  private:
    string cwd_;        // cwd to spawn the process in
    int timeout_;       // max execution time
    int output_limit_;  // max data to receive from child
    string stdout_;     // stdout from the child
    string stderr_;     // stderr from the child
    int out_pipe_[2];   // pipe for child -> parent output
    int err_pipe_[2];   // pipe for child -> parent error

    // helpers for pipe names
    int &out_read_;
    int &out_write_;
    int &err_read_;
    int &err_write_;

    // hidden properties
    subprocess(const subprocess &rhs);              // copy ctor
    subprocess &operator=(const subprocess &rhs);   // assignment operator

  };
  
}
