# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file.
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.12)

set(TDL_DIR "${CMAKE_CURRENT_LIST_DIR}")
include("${TDL_DIR}/CMakeLists.txt")
set(TDL_LIBRARIES "yaml-cpp")
set(TDL_INCLUDE_DIRS "${TDL_DIR}/src")
set(TDL_FOUND TRUE)
