# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/deNBI-cibi/tool_description_lib/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.14)
project (tdl_test_unit CXX)

find_package (tdl REQUIRED HINTS ${CMAKE_CURRENT_LIST_DIR}/../build_system)
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include (GoogleTest OPTIONAL)
include (tdl_fetch_google_test)
tdl_fetch_google_test ()

include (add_tdl_test)
add_tdl_test (ParamValue_test.cpp)
add_tdl_test (Param_test.cpp)
add_tdl_test (ParamCTDFile_test.cpp)

add_executable (test_multiple_definition multiple_definition01_test.cpp multiple_definition02_test.cpp)
target_link_libraries (test_multiple_definition tdl::tdl "gtest_main" "gtest")
add_test (NAME test_multiple_definition COMMAND test_multiple_definition)
