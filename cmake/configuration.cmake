# SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
# SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
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
    # !WORKAROUND A release would be preferred, but the master branch includes various fixes (bugs, CMake, Compiler).
    GIT_TAG 84459a7f982ea4d10e943237b2e9c71afdab6a45
    SYSTEM TRUE
    OPTIONS "YAML_CPP_BUILD_CONTRIB OFF"
            "YAML_CPP_BUILD_TOOLS OFF"
            "YAML_BUILD_SHARED_LIBS OFF"
            "YAML_CPP_INSTALL OFF"
            "YAML_CPP_BUILD_TESTS OFF"
            "BUILD_TESTING OFF" # Disables CMake/CTest/CDash targets (Continuous, Experimental, Nightly, etc.).
)

add_library (tdl STATIC ${tdl_SOURCE_DIR}/src/tdl/convertToCTD.cpp ${tdl_SOURCE_DIR}/src/tdl/convertToCWL.cpp)
target_include_directories (tdl PUBLIC "$<BUILD_INTERFACE:${tdl_SOURCE_DIR}/src>"
                                       "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries (tdl PUBLIC yaml-cpp::yaml-cpp)
target_compile_features(tdl PUBLIC cxx_std_17)
add_library (tdl::tdl ALIAS tdl)

if (INSTALL_TDL)
    include (${tdl_SOURCE_DIR}/cmake/install.cmake)
endif ()
