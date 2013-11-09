#include "common.hpp"
#include "subprocess.hpp"
#include "daemon.hpp"

using namespace vwatch;

/// @brief default ctor
subprocess::subprocess() :
    retcode_(0),
    out_read_(out_pipe_[0]),
    out_write_(out_pipe_[1]),
    err_read_(err_pipe_[0]),
    err_write_(err_pipe_[1]) {
  // create pipes for communication
  pipe(out_pipe_);
  pipe(err_pipe_);

}

subprocess::~subprocess() {
    // close the remaining file descriptors for this process
  close(out_read_);
  close(err_read_);
}

/// @brief Spawn a subprocess on the local machine
/// @param path           Path to executable to launch
/// @param stdio          Handle process stdout and stderr
/// @param args           Arguments to pass to the subprocess
/// @param timeout        Maximum time for the process to execute
/// @param output_limit   Maximum output to receive from child
subprocess *subprocess::spawn(const string &path, subproc_stdio &stdio,
                              const string args, const string cwd,
                              int timeout, const size_t output_limit) {
  // helpers
  subprocess *retval = new subprocess;    // subprocess object
  pid_t pid;                              // pid of the child
  char *new_env[] = { NULL };             // subprocess environment
  std::list <const char *> tmp_args;

  // convert arg string to array
  int prevDelimiter = 0;
  int curPos = 0;
  for (string::const_iterator it = args.begin(); it != args.end(); ++it, ++curPos) {
    if (*it == ' ') {
      tmp_args.push_back(args.substr(prevDelimiter, curPos).c_str());
      prevDelimiter = curPos;
    }
  }
  const char **raw_argv = (const char **)calloc(tmp_args.size(), sizeof(char *)); // allocate arg array
  while (!tmp_args.empty()) {
    // insert each arg (in reverse order)
    raw_argv[tmp_args.size()] = tmp_args.back();
    tmp_args.pop_back();
  }
  
  // initialize the new subprocess object
  retval->path_ = path;
  retval->args_ = args;
  retval->timeout_ = timeout;
  retval->output_limit_ = output_limit;

  // fork to the child process
  if ((pid = fork()) < 0) {
    // parent: fork failure

    syslog(LOG_CRIT, "Error: Unable to fork child process.  Reason: %s\n", strerror(errno));
    daemon::shutdown();
    exit(EXIT_FAILURE);

  } else if (pid == 0) {
    // now in the child process

    // change cwd
    if ((chdir(cwd.c_str())) < 0) {
      cerr << "Error:  Unable to change subprocess directory.  Reason: " << strerror(errno) << endl;
      return NULL;
    }

    // close all open FDs except pipes for writing
    for (int i = getdtablesize(); i >= 0; --i) {
      if (i != retval->out_write_ &&
        i != retval->err_write_) {
        close(i);
      } else {
        // syslog(LOG_CRIT, "open fd: %d", i);
      }
    }

    // duplicate pipes to stdio
    dup2(retval->out_write_, STDOUT_FILENO);
    dup2(retval->err_write_, STDERR_FILENO);
    execve(retval->path_.c_str(), const_cast <char * const *>(raw_argv), new_env);

    // never reached
    exit(EXIT_SUCCESS);
  }

  // parent: successfully forked

  // close remote end of the pipes
  close(retval->out_write_);
  close(retval->err_write_);

  // copy all output to the supplied arguments
  char *buf = new char[1024];
  int sz = 1;
  int err = 0;
  while ((sz < 0 && err == EINTR) || sz > 0) {
    // while reading and not interrupted
    // read from stdout
    sz = ::read(retval->out_read_, reinterpret_cast <void *>(buf), 1024);
    if (sz < 0) {
      if ((err = errno) == EINTR) {
        // system interrupt; check for cancellation and retry
        syslog(LOG_NOTICE, "EINTR received while reading pipe: %s.", strerror(err));
        continue;
      }
      const char *serr = strerror(errno);
      syslog(LOG_NOTICE, "Error reading from subproc pipe: %s.", serr);
    } else if (sz > 0) {
      syslog(LOG_NOTICE, "subproc output: %s (%d bytes).\n", buf, sz);
    }
    if (!err)
      syslog(LOG_NOTICE, "read %d bytes: %s.\n", sz, buf);
  }

  delete[] buf;
  return retval;
}

/// @brief   Get stdout and stderr
/// @param   out  StdOut produced by the child
/// @param   err  StdErr produced by the child
/// @param   ret  Return code produce by the child
/// @return  0 upon successful completion
///          1 if timeout expired
///          2 if output size threshold was exceeded
int subprocess::get_complete_output(string &out, string &err, int &retcode) const
{
  out = stdout_;
  err = stderr_;

  return 0;
}

/// @brief   Display the status of a subprocess
/// @return  Human readble description of the subprocess and state
///          1 if timeout expired
///          2 if output size threshold was exceeded
string subprocess::get_status() const
{
  string retval;
  retval += "Subprocess:\n";
  retval += "-----------\n";
  retval += "Name: asdf\n";
  retval += "Pid: 123\n";
  retval += "Parent Pid: 123\n";
  retval += "Time Running: 10s\n";
  retval += "Bytes Received: 2048\n";
  retval += "State: Running\n";
  retval += "ExitCode: (null)\n";
  return retval;
}
