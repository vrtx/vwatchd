///////////////////////////////////////////////////////
// vwatchd
//
// Copyright (c) 2011 Benjamnin Becker
// All Rights Reserved
//
// @brief   Daemonizing code
// @author  ben
// @date    11/3/2011
///////////////////////////////////////////////////////

#ifndef DAEMON_HPP_VWATCHD
#define DAEMON_HPP_VWATCHD

#define PIDFILE_PATH "/tmp/vwatchd.pid"

namespace vwatch {
    
    /// @class daemon  Daemonize the vwatchd process
    class daemon {
    public:
          static void init();  
          static void shutdown();  
    private:
          static int pidfile;
    };
    
}



#endif
