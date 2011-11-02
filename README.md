vwatchd - Virtualeyes Watching Daemon
---
vwatchd is a daemon that collects and records system information for analysis by virtualeyes.  The 
underlying mechanism for collection is SystemTap for Linux or DTrace for Mac OS and BSD.  The daemon 
collects detailed system information in a noninvasive manner that should require minimal upfront 
configuration by the end user.


Status
---
Just getting started.  


Build instructions
---
This project uses the CMake build system.  To configure and build virtualeyes:

        $ cd vwatchd/build/
        $ cmake ../
        $ make

To install locally, simply run 'make install'.
To build a redistributable dmg/rpm/deb package, run 'make package'.
To run the unit tests, run 'make test'.
To run from the build directory, simply run './src/vwatchd'.


Build Dependancies
---
  - Boost Libraries (1.4.1+)
  - MongoDB C++ Driver (v2.0+)


Runtime Dependancies
---
  - Linux:   SystemTap and debug symbols for the running kernel.
  - MacOS:   DTrace (included with XCode).
  - FreeBSD: DTrace and a kernel built with KDTRACE\_HOOKS, KDTRACE\_FRAME and DDB\_CTF.


Configuration
---
The only required configuration is the DB server in vservd.conf.  


Virtualeyes Monitoring Architecture
---
vwatchd is the data collection component of the Virtualeyes suite.  The kernel watcher records data 
like running processes, open files, active network connections, cpu statistics, etc.  This data is 
saved directly to a storage server (e.g. MySQL, MongoDB, etc.).

The client application, Virtualeyes, currently reads all neccesary data directly from the database.

Another component, vservd, is in the planning phase with the following goals:

 - Proxy data requests to the DB server
 - Perform basic data clustering and statistics
 - Destroy redundant and historic data (once it has been added to any relevant statistical datasets)
 - Render graphs for easy web integration and server-side rendering (Qt painter)
 - Provide real-time alerts


Coding Conventions
------------------
 - Keep names short, but never sacrifice meaning
 - All class/function/variable names should be lower case and use '\_' to seperate words (e.g. some\_var)
 - Member variables have trailing underscores (e.g. some\_member\_var\_)
 - Use literate-style comments
 - Conventions are great, but consistency is king!


TODO
----
Windows support utilizing something akin to SysInternals ProcMon or LiveKD.
Investigate support for a debug symbol server to be utilized with SystemTap.
