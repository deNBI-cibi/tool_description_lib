#pragma once

#include <string>
#include <string_view>

namespace tdl {

//!TODO needs a more general solution
#define TDL_PRETTY_FUNCTION __PRETTY_FUNCTION__

/** simple class to capture __FILE__, __LINE__ and TDL_PRETTY_FUNCTION
 *
 * typical usage:
 * throw TDLEXCEPTION(Source{ __FILE__, __LINE__, TDL_PRETTY_FUNCTION}), "Something went wrong")
 */
class Source {
    std::string_view file;
    int              line;
    std::string_view function;

public:
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


// !This exception is being thrown if any conversion errors have occurred
class ConversionError {

    Source source;
    std::string message;
public:
    ConversionError(Source _source, std::string _message)
        : source{_source}
        , message{std::move(_message)}
    {}

    auto getSource() const -> Source const& {
        return source;
    }
    auto getMessage() const -> std::string_view {
        return message;
    }
};

}
