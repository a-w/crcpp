# CMakeLists.txt
#
# This is the CMake Build file for
#   Dr.Crc's crc test utility
#
# For information on CMake please see http://www.cmake.org
#
# Original Author: Adrian Weiler <dr.crc@crc.guru>
#
# (c) 2014 Adrian Weiler

# project properties
project(crcpp)
set(EXE_NAME crc)

# cmake 2.8 required
cmake_minimum_required(VERSION 2.8)

# Path to public API
include_directories(inc)

# Define a list of headers/sources to use

set(API_HEADERS 
    inc/crc.h inc/crcstream.h
)
source_group("Public API" FILES ${API_HEADERS})

set(EXE_HEADERS 
    src/ICRCAlgorithm.h src/ICRCFactory.h src/ICRCInfo.h
    src/CRCAlgorithm.h  src/CRCFactory.h  src/CRCInfo.h
)

set(EXE_SRCS
    src/crc.cpp
)


if(WIN32)
	include_directories(src/win)

	set(EXE_HEADERS ${EXE_HEADERS}
		src/win/getopt.h
	)

	set(EXE_SRCS ${EXE_SRCS}
        src/win/getopt_long.c
	)
endif(WIN32)

source_group("CRC test utility" FILES ${EXE_HEADERS} ${EXE_SRCS})

# add the executable
add_executable(${EXE_NAME} ${API_HEADERS} ${EXE_HEADERS} ${EXE_SRCS})

# build documentation
add_subdirectory(doc)

# add the unit test
add_subdirectory(UTest)
