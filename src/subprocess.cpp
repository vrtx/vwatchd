///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   Process Spawning code
// @author  ben
// @date    11/3/2011
///////////////////////////////////////////////////////

#include "common.hpp"
#include "spawn.hpp"

using namespace vwatch;

/// @brief Spawn a subprocess on the local machine
/// @param path           Path to executable to launch
/// @param args           Arguments to pass to the subprocess
/// @param out            String of stdout returned by subprocess
/// @param out_err        String of stderr returned by subprocess
/// @param timeout        Maximum time for the process to execute
/// @param output_limit   Maximum output to receive from child
int subprocess::spawn(const string &path, const string &args, 
                      string &out, string &out_err,
                      int timeout, const size_t output_limit)
{
    pid_t pid;

    // create pipes for communication
    pipe(out_pipe);
    pipe(err_pipe);
    
    // fork to the child process
    if ((pid = fork()) < 0) {
        // parent: fork failure

        syslog(LOG_CRIT, "Error: Unable to fork child process.  Reason: %s\n", strerror(errno));
        daemon::shutdown();
        exit(EXIT_FAILURE);

    } else if (pid > 0) {
        // parent: successfully forked

        // close remote end of the pipes
        close(out_write);
        close(err_write);

        // wait for process to finish
//        pthread_join(); //timed
    }
    
    // now in the child process
    // close all open FDs except pipes for writing
    for (int i = getdtablesize(); i >= 0; --i) {

        if (i != out_write &&
            i != err_write) {
            close(i);
        }

    }

    // duplicate pipes to stdio
    dup2(out_write, STDOUT);
    dup2(err_write, STDERR);

    // execve(path.c_str(), ...........);

}

/// @brief   Close any FDs the child should not inherit
/// @param   out  StdOut produced by the child
/// @param   err  StdErr produced by the child
/// @return  0 upon successful completion
///          1 if timeout expired
///          2 if output size threshold was exceeded
int subprocess::get_output(string &out, string &err)
{

    return 0;
}


