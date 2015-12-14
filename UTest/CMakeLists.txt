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

# as of CMake 3.3 we need to prepare FindCxxTest.cmake to use "our" CXX_TEST_PATH
set(CXXTEST_INCLUDE_DIR ${CXXTEST_PATH})
set(CXXTEST_PYTHON_TESTGEN_EXECUTABLE ${CXXTEST_PATH}/bin/cxxtestgen)

find_package(CxxTest QUIET)

if (CXXTEST_FOUND)
    message(STATUS "CxxTest found. Will build unit tests: ${CXXTEST_INCLUDE_DIRS}")
    
    # Different versions of FindCxxTest.cmake output different variables

    if ("${CXXTEST_TESTGEN_EXECUTABLE}" STREQUAL "" AND NOT("${CXXTESTGEN}" STREQUAL ""))
        set(CXXTEST_TESTGEN_EXECUTABLE ${CXXTESTGEN})
    endif()

    if ("${CXXTEST_INCLUDE_DIRS}" STREQUAL "" AND NOT("${CXXTEST_INCLUDES}" STREQUAL ""))
        set(CXXTEST_INCLUDE_DIRS ${CXXTEST_INCLUDES})
    endif()

    # project properties
    set(EXE_NAME UTest)

    # public API
    include_directories(../inc)
    
    # Test cases
    include_directories(.)
    
    # cxxtest
    include_directories(SYSTEM "${CXXTEST_INCLUDE_DIRS}")

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
        COMMAND ${PYTHON_EXECUTABLE} ${CXXTEST_TESTGEN_EXECUTABLE} --runner=ErrorPrinter --output="${CPP_FULL_NAME}" --world=CRC++ "${CMAKE_CURRENT_SOURCE_DIR}/CRCTest.h"
        DEPENDS "${EXE_HEADERS}"
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
    message(WARNING "CxxTest:  No CxxTest installation found. Unit test will not be built.")
    message(STATUS "CxxTest:  If it is installed, provide the full path to it with  -DCXXTEST_PATH=<install-path>")
    message(STATUS "CxxTest:  You can acquire it from: https://sourceforge.net/projects/cxxtest/files/cxxtest/")
endif()
