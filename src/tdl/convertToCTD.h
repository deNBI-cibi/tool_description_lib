// SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <map>
#include <string>

#include "ToolInfo.h"

namespace tdl {

namespace detail {
//!\brief Simple structure representing an XML document (or parts of it)
struct XMLNode {
    std::string                                       tag{};      //!\brief tag of the xml Node, e.g. "div" for <div>
    std::map<std::string, std::optional<std::string>> attr{};     //!\brief a list of attributes included in the tag
    std::vector<XMLNode>                              children{}; //!\brief a list of child XMLNodes
    std::optional<std::string>                        data{};     //!\brief plain data included in this tag

    //!\brief Converts this structures to a string
    auto generateString(std::vector<std::string> const & order, std::string const & ind = "") const -> std::string;
};

auto convertToCTD(Node const & param) -> XMLNode;
} // namespace detail

auto convertToCTD(ToolInfo const & doc) -> std::string;

} // namespace tdl
