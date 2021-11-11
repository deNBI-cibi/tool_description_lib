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
