///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
///////////////////////////////////////////////////////

#pragma once

namespace vwatch {

  // monolithic settings structure
  typedef struct {
    bool daemon_mode;
    bool debug_mode;
    bool syslog;
  } settings_t;

  // Runtime and dynamic configuration system for
  // single and clustered nodes
  class settings {
  public:
    static settings_t runtime;    
    static void parse_cli_args(const int argc, char *argv[]);
  };

}
