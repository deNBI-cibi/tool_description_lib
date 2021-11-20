// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/deNBI-cibi/tool_description_lib/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides platform and dependency checks.
 * \author Svenja Mehringer <svenja.mehringer AT fu-berlin.de>
 * \cond
 */

#pragma once

#include <cinttypes>
#include <ciso646> // makes _LIBCPP_VERSION available
#include <cstddef> // makes __GLIBCXX__ available

// ============================================================================
//  Dependencies
// ============================================================================

// TDL [required]
#if __has_include(<tdl/version.h>)
#    include <tdl/version.h>
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

// ============================================================================
//  OpenMS comptibility
// ============================================================================

// exceptions
// todo: if OpenMS available set OPENMS_EXCEPTIONS_AVAILABLE

// log stream
// This allows OpenMS to set their own LogStream "#include <OpenMS/Concept/LogStream.h> must be included before
// including this header file and all three macros should be defined
#ifndef TDL_LOGSTREAM_TYPE
#   include <iostream>
#   define TDL_LOGSTREAM_TYPE std::ostream
#endif

#ifndef TDL_LOGSTREAM_WARN
#   include <iostream>
#   define TDL_LOGSTREAM_WARN std::cerr
#endif

#ifndef TDL_LOGSTREAM_DEBUG
#   include <iostream>
#   define TDL_LOGSTREAM_DEBUG std::cerr
#endif

// thread critical
// todo: if OpenMS is available include OPENMS_THREAD_CRITICAL macro
//!\endcond
