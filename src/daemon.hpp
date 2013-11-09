///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   Daemonizing code
///////////////////////////////////////////////////////

#pragma once

#define PIDFILE_PATH "/tmp/vwatchd.pid"

namespace vwatch {
    
  // daemonize the vwatchd process
  class daemon {
  public:
    static void init();  
    static void shutdown();  
  private:
    static FILE *pidfile;
  };
    
}
