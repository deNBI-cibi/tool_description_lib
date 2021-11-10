#pragma once

#include <cinttypes>
#include <ciso646> // makes _LIBCPP_VERSION available
#include <cstddef> // makes __GLIBCXX__ available

/*!\file
 * \brief Provides platform and dependency checks.
 * \author Svenja Mehringer <svenja.mehringer AT fu-berlin.de>
 */

// ============================================================================
//  C++ standard and features
// ============================================================================

// C++ standard [required]
#ifdef __cplusplus
static_assert(__cplusplus >= 201709, "TDL requires C++20, make sure that you have set -std=c++20.");
#else
#    error "This is not a C++ compiler."
#endif

#if __has_include(<version>)
#    include <version>
#endif

// ============================================================================
//  Dependencies
// ============================================================================

// TDL [required]
#if __has_include(<tdl/version.hpp>)
#    include <tdl/version.hpp>
#else
#    error TDL include directory not set correctly. Forgot to add -I ${INSTALLDIR}/include to your CXXFLAGS?
#endif

// ============================================================================
//  Documentation
// ============================================================================

// Doxygen related
// this macro is a NO-OP unless doxygen parses it, in which case it resolves to the argument
#ifndef TDL_DOXYGEN_ONLY
#    define TDL_DOXYGEN_ONLY(x)
#endif

// ============================================================================
//  DLL Export
// ============================================================================

// TODO needs to be replaced with https://cmake.org/cmake/help/latest/module/GenerateExportHeader.html
#define TDL_DLLAPI
