# CMakeLists.txt
#
# This is the CMake Build file for
#   the CRC++ class documentation
#
# For information on CMake please see http://www.cmake.org
#
# Original Author: Adrian Weiler <dr.crc@crc.guru>
#
# (c) 2015 Adrian Weiler


# cmake 2.8 required
cmake_minimum_required(VERSION 2.8)

set(DOXYGEN_SKIP_DOT TRUE)
find_package(Doxygen QUIET)

if (DOXYGEN_FOUND)
    message(STATUS "Will build documentation using Doxygen ${DOXYGEN_VERSION}")
 
    
    if (WIN32)
        find_package(HTMLHelp QUIET)
    endif()
 
    file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}" DOXY_OUTDIR)
    
    if (HTML_HELP_COMPILER)
        file(TO_NATIVE_PATH "${HTML_HELP_COMPILER}" HHC)
        message(STATUS "Will build HTML help using hhc in ${HHC}")
        set(GENERATE_HTMLHELP "YES")
        set(DOC_OUTPUT "CRC++.chm")
    else()
        set (GENERATE_TREEVIEW "YES")
        set(DOC_OUTPUT "html/index.html")
    endif()

    set(DOXYCFG "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxy.cfg "${DOXYCFG}")
    
    add_custom_target(
        "doc" 
        SOURCES "${DOC_OUTPUT}"
    )
    
    add_custom_command(
        OUTPUT "${DOC_OUTPUT}"
        COMMAND ${DOXYGEN_EXECUTABLE} "${DOXYCFG}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )

else()
    message(WARNING "Doxygen:  No Doxygen installation found. Documentation will not be built.")
    message(STATUS "Doxygen:  You can acquire Doxygen from: http://www.doxygen.org")
endif()
