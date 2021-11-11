# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/deNBI-cibi/tool_description_lib/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------
#
# This CMake module will try to find tdl and its dependencies.  You can use
# it the same way you would use any other CMake module.
#
#   find_package (tdl [REQUIRED] ...)
#
# tdl has the following platform requirements:
#
#   C++17
#
# tdl has the following optional dependencies:
#
#   Xcerex    -- XML parsing
#
# Once the search has been performed, the following variables will be set.
#
#   TDL_FOUND            -- Indicate whether tdl was found and requirements met.
#
#   TDL_VERSION          -- The version as string, e.g. "3.0.0"
#   TDL_VERSION_MAJOR    -- e.g. 3
#   TDL_VERSION_MINOR    -- e.g. 0
#   TDL_VERSION_PATCH    -- e.g. 0
#
#   TDL_INCLUDE_DIRS     -- to be passed to include_directories ()
#   TDL_LIBRARIES        -- to be passed to target_link_libraries ()
#   TDL_DEFINITIONS      -- to be passed to add_definitions ()
#   TDL_CXX_FLAGS        -- to be added to CMAKE_CXX_FLAGS
#
# Additionally, the following [IMPORTED][IMPORTED] targets are defined:
#
#   tdl::tdl          -- interface target where
#                                  target_link_libraries(target tdl::tdl)
#                              automatically sets
#                                  target_include_directories(target $TDL_INCLUDE_DIRS),
#                                  target_link_libraries(target $TDL_LIBRARIES),
#                                  target_compile_definitions(target $TDL_DEFINITIONS) and
#                                  target_compile_options(target $TDL_CXX_FLAGS)
#                              for a target.
#
#   [IMPORTED]: https://cmake.org/cmake/help/v3.10/prop_tgt/IMPORTED.html#prop_tgt:IMPORTED
#
# ============================================================================

cmake_minimum_required (VERSION 3.4...3.12)

# ----------------------------------------------------------------------------
# Set initial variables
# ----------------------------------------------------------------------------

# make output globally quiet if required by find_package, this effects cmake functions like `check_*`
set(CMAKE_REQUIRED_QUIET_SAVE ${CMAKE_REQUIRED_QUIET})
set(CMAKE_REQUIRED_QUIET ${${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY})

# ----------------------------------------------------------------------------
# Greeter
# ----------------------------------------------------------------------------

string (ASCII 27 Esc)
set (ColourBold "${Esc}[1m")
set (ColourReset "${Esc}[m")

if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
    message (STATUS "${ColourBold}Finding tdl and checking requirements:${ColourReset}")
endif ()

# ----------------------------------------------------------------------------
# Includes
# ----------------------------------------------------------------------------

include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)
include (FindPackageHandleStandardArgs)

# ----------------------------------------------------------------------------
# Pretty printing and error handling
# ----------------------------------------------------------------------------

macro (tdl_config_print text)
    if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
        message (STATUS "  ${text}")
    endif ()
endmacro ()

macro (tdl_config_error text)
    if (${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
        message (FATAL_ERROR ${text})
    else ()
        if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
            message (WARNING ${text})
        endif ()
        return ()
    endif ()
endmacro ()

# ----------------------------------------------------------------------------
# Find tdl include path
# ----------------------------------------------------------------------------

# Note that tdl-config.cmake can be standalone and thus TDL_CLONE_DIR might be empty.
# * `TDL_CLONE_DIR` was already found in tdl-config-version.cmake
# * `TDL_INCLUDE_DIR` was already found in tdl-config-version.cmake
#find_path (TDL_SUBMODULES_DIR NAMES submodules/seqan3 HINTS "${TDL_CLONE_DIR}" "${TDL_INCLUDE_DIR}/tdl")

if (TDL_INCLUDE_DIR)
    tdl_config_print ("tdl include dir found:   ${TDL_INCLUDE_DIR}")
else ()
    tdl_config_error ("tdl include directory could not be found (TDL_INCLUDE_DIR: '${TDL_INCLUDE_DIR}')")
endif ()

# ----------------------------------------------------------------------------
# Detect if we are a clone of repository and if yes auto-add submodules
# ----------------------------------------------------------------------------

if (TDL_CLONE_DIR)
    tdl_config_print ("Detected as running from a repository checkout…")
endif ()

#if (TDL_SUBMODULES_DIR)
#    file (GLOB submodules ${TDL_SUBMODULES_DIR}/submodules/*/include)
#    foreach (submodule ${submodules})
#        if (IS_DIRECTORY ${submodule})
#            tdl_config_print ("  …adding submodule include:  ${submodule}")
#            set (TDL_DEPENDENCY_INCLUDE_DIRS ${submodule} ${TDL_DEPENDENCY_INCLUDE_DIRS})
#        endif ()
#    endforeach ()
#endif ()

# ----------------------------------------------------------------------------
# Options for CheckCXXSourceCompiles
# ----------------------------------------------------------------------------

# deactivate messages in check_*
set (CMAKE_REQUIRED_QUIET       1)
# use global variables in Check* calls
set (CMAKE_REQUIRED_INCLUDES    ${CMAKE_INCLUDE_PATH} ${TDL_INCLUDE_DIR} ${TDL_DEPENDENCY_INCLUDE_DIRS})
set (CMAKE_REQUIRED_FLAGS       ${CMAKE_CXX_FLAGS})

# ----------------------------------------------------------------------------
# Force-deactivate optional dependencies
# ----------------------------------------------------------------------------

# These two are "opt-in", because detected by CMake
# If you want to force-require these, just do find_package (zlib REQUIRED) before find_package (tdl)
#option (TDL_NO_ZLIB  "Don't use ZLIB, even if present." OFF)
#option (TDL_NO_BZIP2 "Don't use BZip2, even if present." OFF)

# ----------------------------------------------------------------------------
# Require C++20
# ----------------------------------------------------------------------------

#set (CMAKE_REQUIRED_FLAGS_SAVE ${CMAKE_REQUIRED_FLAGS})
#
#set (CXXSTD_TEST_SOURCE
#    "#if !defined (__cplusplus) || (__cplusplus < 201709L)
#    #error NOCXX20
#    #endif
#    int main() {}")
#
#check_cxx_source_compiles ("${CXXSTD_TEST_SOURCE}" CXX20_BUILTIN)
#
#if (CXX20_BUILTIN)
#    tdl_config_print ("C++ Standard-20 support:    builtin")
#else ()
#    set (CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS_SAVE} -std=c++20")
#
#    check_cxx_source_compiles ("${CXXSTD_TEST_SOURCE}" CXX20_FLAG)
#
#    if (CXX20_FLAG)
#        tdl_config_print ("C++ Standard-20 support:    via -std=c++20")
#    else ()
#        tdl_config_error ("tdl requires C++20, but your compiler does not support it.")
#    endif ()
#
#    set (TDL_CXX_FLAGS "${TDL_CXX_FLAGS} -std=c++20")
#endif ()

# ----------------------------------------------------------------------------
# Require C++ Concepts
# ----------------------------------------------------------------------------

#set (CMAKE_REQUIRED_FLAGS_SAVE ${CMAKE_REQUIRED_FLAGS})

#set (CXXSTD_TEST_SOURCE
    #"static_assert (__cpp_concepts >= 201507);
    #int main() {}")

#set (CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS_SAVE} ${TDL_CXX_FLAGS}")
#check_cxx_source_compiles ("${CXXSTD_TEST_SOURCE}" TDL_CONCEPTS)

#if (TDL_CONCEPTS_FLAG)
    #tdl_config_print ("C++ Concepts support:       builtin")
#else ()
    #tdl_config_error ("tdl requires C++ Concepts, but your compiler does not support them.")
#endif ()

# ----------------------------------------------------------------------------
# thread support (pthread, windows threads)
# ----------------------------------------------------------------------------

#set (THREADS_PREFER_PTHREAD_FLAG TRUE)
#find_package (Threads QUIET)

#if (Threads_FOUND)
#    set (TDL_LIBRARIES ${TDL_LIBRARIES} Threads::Threads)
#    if ("${CMAKE_THREAD_LIBS_INIT}" STREQUAL "")
#        tdl_config_print ("Thread support:             builtin.")
#    else ()
#        tdl_config_print ("Thread support:             via ${CMAKE_THREAD_LIBS_INIT}")
#    endif ()
#else ()
#    tdl_config_print ("Thread support:             not found.")
#endif ()

# ----------------------------------------------------------------------------
# Require SeqAn3
# ----------------------------------------------------------------------------

#find_package (SeqAn3 REQUIRED QUIET
#              HINTS ${CMAKE_CURRENT_LIST_DIR}/../submodules/seqan3/build_system)

#if (SEQAN3_FOUND)
#    tdl_config_print ("Required dependency:        SeqAn3 found.")
#else ()
#    tdl_config_print ("The SeqAn3 library is required, but wasn't found. Get it from https://github.com/seqan/seqan3")
#endif ()

# ----------------------------------------------------------------------------
# ZLIB dependency
# ----------------------------------------------------------------------------

#if (NOT TDL_NO_ZLIB)
#    find_package (ZLIB QUIET)
#endif ()

#if (ZLIB_FOUND)
#    set (TDL_LIBRARIES         ${TDL_LIBRARIES}         ${ZLIB_LIBRARIES})
#    set (TDL_DEPENDENCY_INCLUDE_DIRS      ${TDL_DEPENDENCY_INCLUDE_DIRS}      ${ZLIB_INCLUDE_DIRS})
#    set (TDL_DEFINITIONS       ${TDL_DEFINITIONS}       "-DTDL_HAS_ZLIB=1")
#    tdl_config_print ("Optional dependency:        ZLIB-${ZLIB_VERSION_STRING} found.")
#else ()
#    tdl_config_print ("Optional dependency:        ZLIB not found.")
#endif ()

# ----------------------------------------------------------------------------
# System dependencies
# ----------------------------------------------------------------------------

# librt
if ((${CMAKE_SYSTEM_NAME} STREQUAL "Linux") OR
    (${CMAKE_SYSTEM_NAME} STREQUAL "kFreeBSD") OR
    (${CMAKE_SYSTEM_NAME} STREQUAL "GNU"))
    set (TDL_LIBRARIES ${TDL_LIBRARIES} rt)
endif ()

# libexecinfo -- implicit
check_include_file_cxx (execinfo.h _TDL_HAVE_EXECINFO)
mark_as_advanced (_TDL_HAVE_EXECINFO)
if (_TDL_HAVE_EXECINFO)
    tdl_config_print ("Optional dependency:        libexecinfo found.")
    if ((${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD") OR (${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD"))
        set (TDL_LIBRARIES ${TDL_LIBRARIES} execinfo elf)
    endif ()
else ()
    tdl_config_print ("Optional dependency:        libexecinfo not found.")
endif ()

# ----------------------------------------------------------------------------
# Perform compilability test of platform.h (tests some requirements)
# ----------------------------------------------------------------------------

set (CXXSTD_TEST_SOURCE
     "#include <tdl/platform.h>
     int main() {}")

# using try_compile instead of check_cxx_source_compiles to capture output in case of failure
file (WRITE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.cxx" "${CXXSTD_TEST_SOURCE}\n")

try_compile (TDL_PLATFORM_TEST
             ${CMAKE_BINARY_DIR}
             ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.cxx
             CMAKE_FLAGS         "-DCOMPILE_DEFINITIONS:STRING=${CMAKE_CXX_FLAGS} ${TDL_CXX_FLAGS}"
                                 "-DINCLUDE_DIRECTORIES:STRING=${CMAKE_INCLUDE_PATH};${TDL_INCLUDE_DIR};${TDL_DEPENDENCY_INCLUDE_DIRS}"
             COMPILE_DEFINITIONS ${TDL_DEFINITIONS}
             LINK_LIBRARIES      ${TDL_LIBRARIES}
             OUTPUT_VARIABLE     TDL_PLATFORM_TEST_OUTPUT)

if (TDL_PLATFORM_TEST)
    tdl_config_print ("tdl platform.h build:  passed.")
else ()
    tdl_config_error ("tdl platform.h build:  failed!\n\
                        ${TDL_PLATFORM_TEST_OUTPUT}")
endif ()

# ----------------------------------------------------------------------------
# Finish find_package call
# ----------------------------------------------------------------------------

find_package_handle_standard_args (${CMAKE_FIND_PACKAGE_NAME} REQUIRED_VARS TDL_INCLUDE_DIR)

# Set TDL_* variables with the content of ${CMAKE_FIND_PACKAGE_NAME}_(FOUND|...|VERSION)
# This needs to be done, because `find_package(tdl)` might be called in any case-sensitive way and we want to
# guarantee that TDL_* are always set.
#foreach (package_var FOUND DIR ROOT CONFIG VERSION VERSION_MAJOR VERSION_MINOR VERSION_PATCH VERSION_TWEAK VERSION_COUNT)
#    set (TDL_${package_var} "${${CMAKE_FIND_PACKAGE_NAME}_${package_var}}")
#endforeach ()

# propagate TDL_INCLUDE_DIR into TDL_INCLUDE_DIRS
set (TDL_INCLUDE_DIRS ${TDL_INCLUDE_DIR} ${TDL_DEPENDENCY_INCLUDE_DIRS})

# ----------------------------------------------------------------------------
# Export targets
# ----------------------------------------------------------------------------

if (TDL_FOUND AND NOT TARGET tdl::tdl)
    separate_arguments (TDL_CXX_FLAGS_LIST UNIX_COMMAND "${TDL_CXX_FLAGS}")

    add_library (tdl_tdl INTERFACE)
    target_compile_definitions (tdl_tdl INTERFACE ${TDL_DEFINITIONS})
    target_compile_options (tdl_tdl INTERFACE ${TDL_CXX_FLAGS_LIST})
    target_link_libraries (tdl_tdl INTERFACE "${TDL_LIBRARIES}")
    # include tdl/include/ as -I, because tdl should never produce warnings.
    target_include_directories (tdl_tdl INTERFACE "${TDL_INCLUDE_DIR}")
    # include everything except tdl/include/ as -isystem, i.e.
    # a system header which suppresses warnings of external libraries.
    target_include_directories (tdl_tdl SYSTEM INTERFACE "${TDL_DEPENDENCY_INCLUDE_DIRS}")
    add_library (tdl::tdl ALIAS tdl_tdl)
endif ()

set (CMAKE_REQUIRED_QUIET ${CMAKE_REQUIRED_QUIET_SAVE})

if (TDL_FIND_DEBUG)
  message ("Result for ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
  message ("")
  message ("  CMAKE_BUILD_TYPE            ${CMAKE_BUILD_TYPE}")
  message ("  CMAKE_SOURCE_DIR            ${CMAKE_SOURCE_DIR}")
  message ("  CMAKE_INCLUDE_PATH          ${CMAKE_INCLUDE_PATH}")
  message ("  TDL_INCLUDE_DIR          ${TDL_INCLUDE_DIR}")
  message ("")
  message ("  ${CMAKE_FIND_PACKAGE_NAME}_FOUND                ${${CMAKE_FIND_PACKAGE_NAME}_FOUND}")
#  message ("  TDL_HAS_ZLIB             ${ZLIB_FOUND}")
#  message ("  TDL_HAS_BZIP2            ${BZIP2_FOUND}")
  message ("")
  message ("  TDL_INCLUDE_DIRS         ${TDL_INCLUDE_DIRS}")
  message ("  TDL_LIBRARIES            ${TDL_LIBRARIES}")
  message ("  TDL_DEFINITIONS          ${TDL_DEFINITIONS}")
  message ("  TDL_CXX_FLAGS            ${TDL_CXX_FLAGS}")
  message ("")
  message ("  TDL_VERSION              ${TDL_VERSION}")
  message ("  TDL_VERSION_MAJOR        ${TDL_VERSION_MAJOR}")
  message ("  TDL_VERSION_MINOR        ${TDL_VERSION_MINOR}")
  message ("  TDL_VERSION_PATCH        ${TDL_VERSION_PATCH}")
endif ()
