// SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <algorithm>
#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace tdl {

namespace detail {

//!\brief Helper struct to detect if a datastructure is a vector
template <typename T>
struct is_vector : std::false_type {};

template <typename value_t, typename allocator_t>
struct is_vector<std::vector<value_t, allocator_t>> : std::true_type {};

template <typename T>
static inline constexpr bool is_vector_v = is_vector<T>::value;

/*!\brief Stores a value with optional limits
 */
template <typename T, typename ListType = T>
struct TValue {
    ListType         value{};
    std::optional<T> minLimit{};
    std::optional<T> maxLimit{};

    enum class State {
        Ok,
        LimitsInvalid,
        ValueTooLow,
        ValueTooHigh
    };

    auto state() const {
        if (minLimit && maxLimit && *minLimit > *maxLimit) {
            return State::LimitsInvalid;
        }

        auto check_value = [this](auto const & value) {
            if (minLimit && *minLimit > value) {
                return State::ValueTooLow;
            }

            if (maxLimit && *maxLimit < value) {
                return State::ValueTooHigh;
            }
        };

        if constexpr (is_vector_v<T>) {
            for (auto const & element : value) {
                check_value(element);
            }
        } else {
            check_value(value);
        }

        return State::Ok;
    }
};

/*!\brief Stores a string with optional list of valid strings.
 */
template <typename T, typename ListType = T>
struct TStringValue {
    ListType                      value{};
    std::optional<std::vector<T>> validValues{};

    enum class State {
        Valid,
        Invalid
    };

    auto state() const {
        if (!validValues) {
            return State::Valid;
        }

        auto checkSingleValue = [this](T const & value) {
            bool const is_valid = std::any_of(begin(*validValues), end(*validValues), [&](auto const & pattern) {
                return value == pattern;
            });

            if (!is_valid) {
                return State::Invalid;
            }
        };

        if constexpr (is_vector_v<T>) {
            for (auto const & element : value) {
                checkSingleValue(element);
            }
        } else {
            checkSingleValue(value);
        }

        return State::Valid;
    }
};

} // namespace detail

// Value types that are valid entries in the Node
using BoolValue       = bool;
using IntValue        = detail::TValue<int>;
using DoubleValue     = detail::TValue<double>;
using StringValue     = detail::TStringValue<std::string>;
using IntValueList    = detail::TValue<int, std::vector<int>>;
using DoubleValueList = detail::TValue<double, std::vector<double>>;
using StringValueList = detail::TStringValue<std::string, std::vector<std::string>>;

/*!\brief represents a parameter tree or a subtree of the parameter tree.
 *
 * This represents values that are structured in a tree and strongly typed.
 */
struct Node {
    using Children = std::vector<Node>;
    using Value    = std::variant<BoolValue, // just a single bool value
                               IntValue,  // single int, double or string value
                               DoubleValue,
                               StringValue,
                               IntValueList, // list of int, double or string values
                               DoubleValueList,
                               StringValueList,
                               Children>; // not a value, but a node with children

    std::string           name{};            //!< Name of the entry.
    std::string           description{};     //!< Entry description.
    std::set<std::string> tags{};            //!< List of tags, e.g.: advanced parameter tag.
    Value                 value{Children{}}; //!< Current value of this entry
};

//! A pair of mapping from tree parameter names to cli names
struct CLIMapping {
    std::string optionIdentifier; //!< full name on the command line (including '-' or '--')
    std::string referenceName;    //!< name of the option inside the parameter tree
};

//\brief Citation information of the app
struct Citation {
    std::string doi; //!\brief the doi (document object identifier)
    std::string url; //!\brief an url for direct access.
};

//!\brief Meta data of the tool
struct MetaInfo {
    std::string           version{};        //!\brief version as a string
    std::string           name{};           //!\brief name of the app
    std::string           docurl{};         //!\brief url to the documentation of the app
    std::string           category{};       //!\brief category of the app
    std::string           description{};    //!\brief a brief description of the app
    std::string           executableName{}; //!\brief the actual call of this app
    std::vector<Citation> citations{};      //!\brief list publication integrated into this app
};

//! A full parameter tree document with cli mappings
struct ToolInfo {
    MetaInfo                metaInfo{};
    Node::Children          params{};
    std::vector<CLIMapping> cliMapping{};
};

} // namespace tdl
