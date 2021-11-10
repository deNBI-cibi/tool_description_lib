cmake_minimum_required (VERSION 3.14)

# Exposes the google-test targets `gtest` and `gtest_main`.
macro (tdl_fetch_google_test_test)
    enable_testing ()

    set (gtest_git_tag "release-1.11.0")

    #if (NOT CMAKE_VERSION VERSION_LESS 3.14)
        message (STATUS "Fetch googletest:")

        include (FetchContent)
        FetchContent_Declare (
            gtest_fetch_content
            GIT_REPOSITORY "https://github.com/google/googletest.git"
            GIT_TAG "${gtest_git_tag}"
        )
        option (BUILD_GMOCK "" OFF)
        FetchContent_MakeAvailable(gtest_fetch_content)

    if (NOT TARGET gtest_build)
        add_custom_target (gtest_build DEPENDS gtest_main gtest)
    endif ()
endmacro ()
