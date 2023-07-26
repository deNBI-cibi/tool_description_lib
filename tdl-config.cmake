# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file.
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.12)
if (TARGET tdl)
    return()
endif()

find_package (yaml-cpp QUIET)

if (NOT yaml-cpp_FOUND)
    message (STATUS "Fetching yaml-cpp")

    include (FetchContent)
    FetchContent_Declare (
        yaml-cpp_fetch_content
        GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp.git"
        GIT_TAG "35b4498026b6293bfadc75f9ee325cb16d6975af") # 0.7.0 can't be properly installed
    option (YAML_CPP_BUILD_CONTRIB "" OFF)
    option (YAML_CPP_BUILD_TOOLS "" OFF)
    option (YAML_BUILD_SHARED_LIBS "" OFF)
    option (YAML_CPP_INSTALL "" ON)
    set_property (GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    FetchContent_MakeAvailable (yaml-cpp_fetch_content)
else ()
    message (STATUS "Found yaml-cpp ${yaml-cpp_VERSION}")
endif ()

add_library(tdl INTERFACE)
target_include_directories(tdl INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(tdl INTERFACE yaml-cpp)
add_library (tdl::tdl ALIAS tdl)
