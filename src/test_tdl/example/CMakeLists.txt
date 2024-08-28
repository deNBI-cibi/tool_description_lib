# SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
# SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required (VERSION 3.15)

project (tdl_example CXX)

find_package (tdl REQUIRED PATHS "${TDL_INSTALL_DIR}")

enable_testing ()
add_executable (example1 example1.cpp)
target_link_libraries (example1 tdl::tdl)
add_test (NAME example1 COMMAND example1)

add_executable (example2 example2.cpp)
target_link_libraries (example2 tdl::tdl)
add_test (NAME example2 COMMAND example2)
