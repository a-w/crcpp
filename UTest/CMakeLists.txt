# CMakeLists.txt
#
# This is the CMake Build file for
#   Dr.Crc's unit test
#
# For information on CMake please see http://www.cmake.org
#
# Original Author: Adrian Weiler <dr.crc@crc.guru>
#
# (c) 2015 Adrian Weiler


# cmake 2.8 required
cmake_minimum_required(VERSION 2.8)

if ("${CXXTEST_PATH}" STREQUAL "" AND NOT("$ENV{CXXTEST_PATH}" STREQUAL ""))
  message("Setting CXXTEST_PATH to $ENV{CXXTEST_PATH} (from environment)")
  set (CXXTEST_PATH $ENV{CXXTEST_PATH})
endif()

find_package(CxxTest QUIET)

if (CXXTEST_FOUND)
    message("CxxTest found. Will build unit tests")
    
    # project properties
    set(EXE_NAME UTest)

    # public API
    include_directories(../inc)
    
    # Test cases
    include_directories(.)
    
    # cxxtest
    include_directories(SYSTEM "${CXXTEST_PATH}")

    # Define a list of headers/sources to use
    set(API_HEADERS
        ../inc/crc.h ../inc/crcstream.h
    )

    set(EXE_HEADERS 
        CRCTest.h
    )

    set(EXE_SRCS
        CRCTest.cpp
    )

    set(CPP_FULL_NAME "${CMAKE_CURRENT_BINARY_DIR}/UTest.cpp")

    add_custom_command(
        OUTPUT "${CPP_FULL_NAME}"
        COMMAND ${CXXTESTGEN} --runner=ErrorPrinter --output="${CPP_FULL_NAME}" --world=CRC++ "${CMAKE_CURRENT_SOURCE_DIR}/CRCTest.h"
        DEPENDS "${API_HEADERS} ${EXE_HEADERS}"
    )
    
    # add the executable
    add_executable(${EXE_NAME} ${API_HEADERS} ${EXE_HEADERS} ${CPP_FULL_NAME} ${EXE_SRCS})
    if (UNIX)
        set_target_properties(${EXE_NAME} PROPERTIES COMPILE_FLAGS "-Wno-effc++")
    endif()

    add_custom_command(
        TARGET ${EXE_NAME}
        POST_BUILD
        COMMAND $<TARGET_FILE:${EXE_NAME}>
    )
else()
    message("No CxxTest installation found. Unit test will not be built")
endif()