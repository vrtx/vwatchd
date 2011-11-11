# vwatchd cmake configuration
#
# Date Created: 10/31/11
# Author: Ben Becker
##############################

# Project setup
project(vwatchd)
cmake_minimum_required(VERSION 2.8.0)

# Definitions
add_definitions(-std=c++0x)
add_definitions(-Wall)
add_definitions(-Werror)
add_definitions(-pedantic)
add_definitions(-g)

# Package Dependancies
find_package(Boost REQUIRED)

# Build options
option(enable_tests "Build the unit test suite." OFF)

# include source and test directories
add_subdirectory(src)
add_subdirectory(tests)
