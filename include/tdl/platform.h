#pragma once

#include <cinttypes>
#include <ciso646> // makes _LIBCPP_VERSION available
#include <cstddef> // makes __GLIBCXX__ available

/*!\file
 * \brief Provides platform and dependency checks.
 * \author Svenja Mehringer <svenja.mehringer AT fu-berlin.de>
 */

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
#include <iostream> // todo: if OpenMS is availabe, #include <OpenMS/CONCEPT/LogStream.h>
#define TDL_LOGSTREAM_TYPE std::ostream // todo if OpenMS is availabe, set this Logger::LogStream
#define TDL_LOGSTREAM_WARN std::cerr // todo: if OpenMS is availabe, set this to OPENMS_LOG_WARN
#define TDL_LOGSTREAM_DEBUG std::cerr // todo: if OpenMS is availabe, set this to OPENMS_LOG_DEBUG

// thread critical
// todo: if OpenMS is available include OPENMS_THREAD_CRITICAL macro
