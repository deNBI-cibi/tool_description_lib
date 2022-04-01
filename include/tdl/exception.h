#pragma once

#include <string>
#include <string_view>

namespace tdl {

//!TODO needs a more general solution
#ifndef TDL_PRETTY_FUNCTION
#define TDL_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

/** Represents a location in the source code.
 *
 * It captures __FILE__, __LINE__ and TDL_PRETTY_FUNCTION.
 *
 * typical usage:
 * throw TDLEXCEPTION(Source{ __FILE__, __LINE__, TDL_PRETTY_FUNCTION}), "Something went wrong")
 */
class Source {
    std::string_view file;
    int              line;
    std::string_view function;

public:
    /**
     * \param _file source file of the throw statement.
     * \param _line line number inside the source file.
     * \param _function the name of the function, inside which the throw statement occurred.
     *
     * Usually this is called as `Source{__FILE__, __LINE__, TDL_PRETTY_FUNCTION}`.
     *
     * NOTE: If this library moves to c++20 as minimum this can be replaced with `std::source_location` and `std::source_location::current()`.
     */
    Source(std::string_view _file, int _line, std::string_view _function)
        : file{_file}
        , line{_line}
        , function{_function}
    {}

    /// Returns the file where it occurred
    auto getFile() const noexcept -> std::string_view {
        return file;
    }

    /// Returns the line number where it occurred
    auto getLine() const noexcept -> int {
        return line;
    }

    /// Returns the function where it occurred
    auto getFunction() const noexcept -> std::string_view {
        return function;
    }
};


/** This exception is being thrown if any conversion errors have occurred
 */
class ConversionError {
    Source source;
    std::string message;

public:
    /**
     * \param _source location where the error occurred.
     * \param _message reason why the exception was thrown.
     */
    ConversionError(Source _source, std::string _message)
        : source{_source}
        , message{std::move(_message)}
    {}

    /// Location where this exception was thrown.
    auto getSource() const -> Source const& {
        return source;
    }

    /// Reason why this exception was thrown.
    auto getMessage() const -> std::string_view {
        return message;
    }
};

} // namespace tdl