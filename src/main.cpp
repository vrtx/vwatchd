///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   Initialize the vwatch daemon
///////////////////////////////////////////////////////

#include "common.hpp"
#include "version.hpp"
#include "signals.hpp"
#include "settings.hpp"
#include "daemon.hpp"

//testing
#include "subprocess.hpp"

using namespace vwatch;

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

  // load config file

  // spawn subprocesses

  // allow other threads to execute (or run io_service?)

  // never reached
  exit(EXIT_SUCCESS);
}
