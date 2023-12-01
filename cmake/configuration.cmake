# SPDX-FileCopyrightText: 2006-2023, Knut Reinert & Freie Universität Berlin
# SPDX-FileCopyrightText: 2016-2023, Knut Reinert & MPI für molekulare Genetik
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required (VERSION 3.12)

if (TARGET tdl)
    return ()
endif ()

option (INSTALL_TDL "Enable installation of TDL. (Projects embedding TDL may want to turn this OFF.)" ON)

include (${tdl_SOURCE_DIR}/cmake/CPM.cmake)

set (TDL_YAML_CPP_VERSION "0.8.0" CACHE STRING "Version of yaml-cpp to use.")

CPMAddPackage (
    NAME yaml-cpp
    VERSION ${TDL_YAML_CPP_VERSION}
    GITHUB_REPOSITORY jbeder/yaml-cpp
    # !WORKAROUND Points to first commit after 0.8.0: Fixes CMake deprecation warnings (cmake_minimum_required)
    GIT_TAG c2680200486572baf8221ba052ef50b58ecd816e # ${TDL_YAML_CPP_VERSION}
    SYSTEM TRUE
    OPTIONS "YAML_CPP_BUILD_CONTRIB OFF"
            "YAML_CPP_BUILD_TOOLS OFF"
            "YAML_BUILD_SHARED_LIBS OFF"
            "YAML_CPP_INSTALL OFF"
            "YAML_CPP_BUILD_TESTS OFF")

add_library (tdl INTERFACE)
target_include_directories (tdl INTERFACE "$<BUILD_INTERFACE:${tdl_SOURCE_DIR}/src>"
                                          "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries (tdl INTERFACE yaml-cpp::yaml-cpp)
add_library (tdl::tdl ALIAS tdl)

if (INSTALL_TDL)
    include (${tdl_SOURCE_DIR}/cmake/install.cmake)
endif ()
