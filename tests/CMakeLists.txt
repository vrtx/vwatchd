# vwatchd cmake configuration file for unit tests
#
# Date Created: 11/7/11
# Author: Ben Becker
##############################

# Project setup
project(vwatchd)

if(enable_tests)

    # # Include vwatchd headers and objects
    # link_directories(${CMAKE_BINARY_DIR}/src)
    # include_directories(${CMAKE_SOURCE_DIR}/src)

    enable_testing()

    add_executable(unittests
        _boost_test_runner.cpp
        subprocess_tests.cpp)

    target_link_libraries(unittests
        boost_unit_test_framework-mt
        pthread
        vwatchd_core)

    ADD_TEST(unittests "VWatchd Unit Tests")

    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)

endif()
