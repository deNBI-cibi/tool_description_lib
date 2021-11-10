cmake_minimum_required (VERSION 3.10)
project (tdl_test_unit CXX)

find_package (tdl REQUIRED HINTS ${CMAKE_CURRENT_LIST_DIR}/../build_system)

include (GoogleTest OPTIONAL)

include (${CMAKE_CURRENT_LIST_DIR}/cmake/tdl_fetch_google_test_test.cmake)
include (${CMAKE_CURRENT_LIST_DIR}/cmake/add_tdl_test.cmake)

tdl_fetch_google_test_test ()

# Add tests below

add_tdl_test(ParamValue_test.cpp)
add_tdl_test(Param_test.cpp)
