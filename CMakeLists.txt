# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file.
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.15)

# check if this is the main project
if (NOT DEFINED PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(TDL_ROOT_PROJECT TRUE)
endif ()

project (tdl LANGUAGES CXX VERSION "${TDL_PROJECT_VERSION}"
             DESCRIPTION "tdl -- tool description library")

if (NOT TARGET tdl)
    add_library(tdl INTERFACE)
    target_include_directories(tdl INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
        $<INSTALL_INTERFACE:include>)
endif()

find_package (yaml-cpp QUIET)

if (NOT yaml-cpp_FOUND)
    message (STATUS "Fetching yaml-cpp")

    include (FetchContent)
    FetchContent_Declare (
        yaml-cpp_fetch_content
        GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp.git"
        GIT_TAG "yaml-cpp-0.7.0")
    option (YAML_CPP_BUILD_CONTRIB "" OFF)
    option (YAML_CPP_BUILD_TOOLS "" OFF)
    option (YAML_BUILD_SHARED_LIBS "" OFF)
    option (YAML_CPP_INSTALL "" ON)
    set_property (GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    FetchContent_MakeAvailable (yaml-cpp_fetch_content)
    target_compile_options (yaml-cpp PRIVATE "-w")
else ()
    message (STATUS "Found yaml-cpp ${yaml-cpp_VERSION}")
endif ()

target_link_libraries(tdl INTERFACE yaml-cpp)
add_library (tdl::tdl ALIAS tdl)

if (${TDL_ROOT_PROJECT})
    enable_testing()
    file(GLOB TEST_CPP_FILES
         LIST_DIRECTORIES false
         RELATIVE ${PROJECT_SOURCE_DIR}
         CONFIGURE_DEPENDS
         src/test_tdl/*.cpp)
    add_executable(test_tdl ${TEST_CPP_FILES})
    target_link_libraries(test_tdl tdl)
    add_test(NAME test_tdl COMMAND test_tdl)
endif()
