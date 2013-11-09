[vwatchd] - the virtualeyes data collector
---
The vwatchd daemon collects process and system information from the local host.  This information includes:

 - System information (CPUs, network interfaces, disks, mount points, etc.)
 - CPU metrics (e.g. cache misses, NUMA memory fetch, context switches, etc.)
 - OS information (running processes, disk IO, scheduler stats, etc.)
 - Process information (NUMA maps, memory mappings, open files, sockets, etc.)
 - Process traces (e.g. function hooks, buffers, signals, etc.)

This information is saved to disk to satisfy future queries for the state of the host as it existed
at any point (or range) in time.


Build instructions
---
This project uses the CMake build system.  To configure and build virtualeyes:

        $ cd vwatchd/build/
        $ cmake ../
        $ make

At this point you can launch ./src/vwatchd

To install:  (optional)

		$ make install

To build and run the unit tests:  (optional)

		$ make test

To build a redistributable dmg/rpm/deb package:  (optional)

		$ make package


Build Dependancies
---
  - MongoDB C++ Driver (v2.0+)
  - Boost Libraries (1.4.1+) (required for mongo)
  - libptrace


Project Status
---
Prototype.  Give it time.


How to Run
---
After building and installing vwatchd, simply start the daemon (via upstart, launchd, or just
run ./vwatchd).  To configure a remote host or custom file path, simply edit /etc/vwatchd.conf.
Once startd, vwatchd will write to the specified file or host (default is /vwatchd.db).


How to Trace
---
TODO:

 - attach to a running process
 - start a process
 - add/remove/list probes


How to Cluster
---
To connect a node to an existing cluster, simply add a remote-host entry for any existing node
to the config file.  This node will operate as part of that cluster.


How to Analyze
---
Use the virtualeyes application to analyze a cluster, host, process, etc.  Eventually, data may
be collected from a console using a query or scripting language (e.g. lua, javascript, SQL or a DSL).


How to Alert
---
TODO: either a JSON file w/ inotify, or a syntax like:

 - show alerts
 - add group admins user1@email, user2@email, ...
 - add alert on [event x|threshold y|observation z] from [host <host> |any host] for [user@email|310-555-1212|admins]


How to Add a Probe
---
TODO:

 - Process ...
 - System ...


Note About Configuration
---
Cluster-wide configuration parameters are automatically replicated, and can be modified at any time
by updating '/etc/vwatchd.cluster.json' in your favorite editor.  Changes will be applied and replicated
any time this file is saved.


Coding Conventions
------------------
 - Avoid RTTI
 - Avoid Exceptions (see the Status class)
 - Keep names short, but never sacrifice meaning
 - Classes, namespaces, functions and variables should be lower case and use '\_' to seperate words (e.g. some\_var)
 - Member variables have trailing underscores (e.g. some\_member\_var\_)
 - Prefer constants and enums over MACROS
 - Comment .hpp files to describe intent
 - Be Consistent
