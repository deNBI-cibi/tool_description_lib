# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/deNBI-cibi/tool_description_lib/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.15)

# Exposes the google-test targets `gtest` and `gtest_main`.
macro (tdl_fetch_google_test)
    enable_testing ()

    message (STATUS "Fetch Google Test:")

    include (FetchContent)
    FetchContent_Declare (
        gtest_fetch_content
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG "8d51dc50eb7e7698427fed81b85edad0e032112e"
    )
    option (BUILD_GMOCK "" OFF)
    FetchContent_MakeAvailable (gtest_fetch_content)

    if (NOT TARGET gtest_build)
        add_custom_target (gtest_build DEPENDS gtest_main gtest)
    endif ()

endmacro ()
