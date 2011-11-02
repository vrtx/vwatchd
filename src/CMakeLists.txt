# vwatchd cmake configuration
#
# Date Created: 10/31/11
# Author: Ben Becker
##############################

# Project setup
project(vwatchd)

# Core source files
FILE(GLOB ALL_CPP_SRCS *.cpp)
SET(SRCS
    ${ALL_CPP_SRCS})

# Header files
FILE(GLOB ALL_CPP_HDRS *.hpp)
SET(HDRS
    ${ALL_CPP_HDRS})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src
                    ${CMAKE_SOURCE_DIR}/src/core)

# platform-specific executable targets
IF(UNIX)
    IF(APPLE)
        # MacOS Application Bundle
        add_definitions(-DVWATCH_DTRACE)
        add_executable(vwatchd MACOSX_BUNDLE ${HDRS} ${SRCS})
    ELSE()
        IF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD|OpenBSD|NetBSD|Solaris|SunOS")
            # *BSD or Solaris
            add_definitions(-DVWATCH_DTRACE)
            add_executable(vwatchd ${HDRS} ${SRCS})
        ELSE()
            # Linux
            add_definitions(-DVWATCH_STAP)
            add_executable(vwatchd ${HDRS} ${SRCS})
        ENDIF()
    ENDIF()
ELSE()
    IF(WIN32)
        # Windows Executable
        add_definitions(-DVWATCH_WINMON)
        add_executable(vwatchd ${HDRS} ${SRCS})
    ENDIF()
ENDIF()

# link libs to executable
TARGET_LINK_LIBRARIES(vwatchd
    pthread
)

# set the installer
INSTALL(TARGETS vwatchd DESTINATION sbin)
