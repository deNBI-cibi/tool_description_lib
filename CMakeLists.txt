# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/deNBI-cibi/tool_description_lib/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.12)

find_path (TDL_MODULE_PATH "tdl-config.cmake" HINTS "${CMAKE_CURRENT_LIST_DIR}/build_system/")
list (APPEND CMAKE_MODULE_PATH "${TDL_MODULE_PATH}")

include (tdl-config-version)

project (
    tdl LANGUAGES CXX VERSION "${TDL_PROJECT_VERSION}"
    DESCRIPTION "tdl -- tool description library"
    HOMEPAGE_URL "https://github.com/deNBI-cibi/tool_description_lib"
)

find_package (tdl 0.1 REQUIRED HINTS ${TDL_MODULE_PATH})
