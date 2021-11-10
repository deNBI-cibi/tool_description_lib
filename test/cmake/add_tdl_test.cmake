cmake_minimum_required (VERSION 3.10)
#project (tdl_test_unit CXX)

#find_package (tdl REQUIRED HINTS ${CMAKE_CURRENT_LIST_DIR}/../build_system)

#include (GoogleTest OPTIONAL)

# A compatible function for cmake < 3.20 that basically returns `cmake_path (GET <filename> STEM LAST_ONLY <out_var>)`
function (tdl_path_longest_stem out_var filename)
    if (CMAKE_VERSION VERSION_LESS 3.20)  # cmake < 3.20
        get_filename_component (result "${filename}" NAME)
        if (result MATCHES "\\.")
            string (REGEX REPLACE "(.+)[.].*" "\\1" result "${result}")
        endif ()
    else () # cmake >= 3.20
        cmake_path (GET filename STEM LAST_ONLY result)
    endif ()

    set ("${out_var}" "${result}" PARENT_SCOPE) # out-var
endfunction ()

macro (add_tdl_test unit_test_cpp)
    cmake_parse_arguments(TDL_TEST "" "" "CYCLIC_DEPENDING_INCLUDES" ${ARGN})

    file (RELATIVE_PATH unit_test "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_LIST_DIR}/${unit_test_cpp}")

    tdl_path_longest_stem (target ${unit_test}) # set target name
    set (test_name ${target})

    add_executable (${target} ${unit_test_cpp})
    target_link_libraries (${target} tdl::tdl "gtest_main" "gtest")

    add_test (NAME "${test_name}" COMMAND ${target})

    unset (unit_test)
    unset (target)
    unset (test_name)
endmacro ()
