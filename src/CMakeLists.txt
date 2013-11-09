# vwatchd cmake build configuration
###################################

# Project setup
project(vwatchd)

# Source files for the vwatchd daemon
FILE(GLOB ALL_CPP_SRCS *.cpp)
SET(SRCS ${ALL_CPP_SRCS})

# Header files
FILE(GLOB ALL_CPP_HDRS *.hpp)
SET(HDRS ${ALL_CPP_HDRS})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src
                    ${CMAKE_SOURCE_DIR}/src/core)

# Platform-specific executable targets
IF(UNIX)
    IF(APPLE)
        # MacOS Application Bundle
        add_executable(vwatchd MACOSX_BUNDLE ${HDRS} ${SRCS})
    ELSE()
        IF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD|OpenBSD|NetBSD|Solaris|SunOS")
            # *BSD or Solaris
            add_executable(vwatchd ${HDRS} ${SRCS})
        ELSE()
            # Linux
            add_executable(vwatchd ${HDRS} ${SRCS})
        ENDIF()
    ENDIF()
ELSE()
    IF(WIN32)
        # Windows Executable
        add_executable(vwatchd ${HDRS} ${SRCS})
    ENDIF()
ENDIF()

# Set up a shared object build for tests
SET(SHARED_SRCS ${SRCS} ${HDRS})
LIST(REMOVE_ITEM SHARED_SRCS main.cpp)
IF (enable_tests)
    add_library(vwatchd_core SHARED ${SHARED_SRCS})
ENDIF()

# Link libraries to executable
TARGET_LINK_LIBRARIES(vwatchd
                                pthread
)

# set the installer
INSTALL(TARGETS vwatchd DESTINATION sbin)
