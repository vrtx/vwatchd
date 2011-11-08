# vwatchd cmake configuration file for unit tests
#
# Date Created: 11/7/11
# Author: Ben Becker
##############################

# Project setup
project(vwatchd)

# Include vwatchd headers and components
enable_testing()
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/src)

# Build an executable file
add_executable(subproc_stdio subproc_stdio.cpp)
target_link_libraries(subproc_stdio vwatchd_core)
add_test(NAME SubprocessStdio 
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/subproc_stdio)

