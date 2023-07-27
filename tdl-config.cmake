# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2023, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2023, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file.
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.12)
if (TARGET tdl)
    return ()
endif ()

option (INSTALL_TDL "Enable installation of TDL. (Projects embedding TDL may want to turn this OFF.)" ON)

find_package (yaml-cpp QUIET)
if (NOT yaml-cpp_FOUND)
    if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
        message (STATUS "Fetching yaml-cpp")
    endif ()

    include (FetchContent)
    FetchContent_Declare (
        yaml-cpp_fetch_content
        GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp.git"
        # !WORKAROUND git tag 0.7.0 can't be properly installed due to missing CMake
        GIT_TAG "35b4498026b6293bfadc75f9ee325cb16d6975af")
    option (YAML_CPP_BUILD_CONTRIB "" OFF)
    option (YAML_CPP_BUILD_TOOLS "" OFF)
    option (YAML_BUILD_SHARED_LIBS "" OFF)
    option (YAML_CPP_INSTALL "" ${INSTALL_TDL})
    option (YAML_CPP_BUILD_TESTS "" OFF)
    set_property (GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    FetchContent_MakeAvailable (yaml-cpp_fetch_content)
else ()
    if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
        message (STATUS "Found yaml-cpp")
    endif ()
endif ()

add_library (tdl INTERFACE)
target_include_directories (tdl INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>"
                                          "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
# !WORKAROUND yaml-cpp::yaml-cpp was only defined after the 0.7.0 release
# yaml-cpp::yaml-cpp is required to correctly link against a fetch_content'ed yaml-cpp
if (TARGET yaml-cpp::yaml-cpp)
    target_link_libraries (tdl INTERFACE yaml-cpp::yaml-cpp)
else ()
    target_link_libraries (tdl INTERFACE yaml-cpp)
endif ()
add_library (tdl::tdl ALIAS tdl)

if (INSTALL_TDL)
    include (${CMAKE_CURRENT_LIST_DIR}/cmake/install.cmake)
endif ()
