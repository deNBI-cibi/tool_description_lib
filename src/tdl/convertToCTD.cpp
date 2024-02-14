// SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: BSD-3-Clause

#include "convertToCTD.h"

#include <cassert>
#include <iomanip>
#include <limits>
#include <sstream>

namespace tdl {

namespace detail {

//!\brief overload structure allowing fancy 'std::visit` syntax
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

//!\brief required deduction guide for c++17 (not required for c++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

//!\brief replaces all occurrences of a character with a string
inline auto replace(std::string const & input, char const old_character, std::string const & new_string) {
    auto out = std::string{};
    out.reserve(input.size());

    for (auto chr : input) {
        if (chr == old_character) {
            out.append(new_string);
        } else {
            out.push_back(chr);
        }
    }

    return out;
}

//!\brief required replacements to use strings safely inside XML documents
inline auto escapeXML(std::string str) {
    str = replace(str, '&', "&amp;");
    str = replace(str, '>', "&gt;");
    str = replace(str, '"', "&quot;");
    str = replace(str, '<', "&lt;");
    str = replace(str, '\'', "&apos;");
    str = replace(str, '\t', "&#x9;");
    str = replace(str, '\n', "#br#");

    return str;
}

//!\brief Converts this structures to a string
auto XMLNode::generateString(std::vector<std::string> const & order, std::string const & ind) const -> std::string {
    auto ss = std::stringstream{};
    ss << ind << "<" << escapeXML(tag);

    // orders the attributes according to the values in `order`
    auto attributes = [&]() {
        auto t_attr = this->attr;
        auto attr2  = std::vector<std::tuple<std::string, std::optional<std::string>>>{};

        for (auto const & o : order) {
            if (t_attr.count(o) > 0) {
                attr2.emplace_back(o, t_attr.at(o));
                t_attr.erase(o);
            }
        }

        for (auto & [key, v] : t_attr) {
            attr2.emplace_back(key, v);
        }

        return attr2;
    }();

    // generate all attributes
    for (auto const & [key, v] : attributes) {
        ss << " " << key;
        if (v) {
            ss << "=\"" << escapeXML(*v) << "\"";
        }
    }

    assert(children.empty() || !data); // can only have children or data, not both

    if (children.empty() && !data) {
        ss << " />\n";
    } else if (children.empty()) {
        ss << ">" << *data << "</" << escapeXML(tag) << ">\n";
    } else {
        ss << ">\n";
        for (auto const & child : children) {
            ss << child.generateString(order, ind + "    ");
        }
        ss << ind << "</" << escapeXML(tag) << ">\n";
    }

    return ss.str();
}

//!\brief convenience function, to have highest precision for float/double values
template <typename T>
auto convertToString(T v) -> std::string {
    auto ss = std::stringstream{};
    ss << std::setprecision(std::numeric_limits<std::decay_t<T>>::max_digits10) << v;
    return ss.str();
}

// helper function, to generate the restriction attribute
template <typename T>
auto generateRestrictions(T const & value) -> std::optional<std::string> {
    if (!value.minLimit && !value.maxLimit) {
        return std::nullopt;
    }

    auto restrictions = std::string{};

    if (value.minLimit) {
        restrictions = convertToString(*value.minLimit);
    }

    restrictions += ':';

    if (value.maxLimit) {
        restrictions += convertToString(*value.maxLimit);
    }

    return restrictions;
}

// helper function to generate the restrictions of string values
template <typename T>
auto generateValidList(T const & value) -> std::optional<std::string> {
    if (!value.validValues || value.validValues->empty()) {
        return std::nullopt;
    }

    auto const & validValues = *value.validValues;

    return std::accumulate(++begin(validValues),
                           end(validValues),
                           *begin(validValues),
                           [](auto acc, auto const & next) {
                               acc.push_back(',');
                               acc.append(next);
                               return acc;
                           });
}

//!\brief converts a Node into an XMLNode
auto convertToCTD(Node const & param) -> XMLNode {
    // generate a single Node
    auto xmlNode = XMLNode{/*.tag = */ "NODE",
                           /*.attr = */ {{"name", param.name}, {"description", param.description}}};

    auto tags = param.tags; // copy tags to make them mutable

    // fill xmlNode depending on the actual type
    std::visit(overloaded{
                   [&](BoolValue value) {
                       xmlNode.tag           = "ITEM";
                       xmlNode.attr["type"]  = "bool";
                       xmlNode.attr["value"] = value ? "true" : "false";
                   },
                   [&](IntValue value) {
                       xmlNode.tag           = "ITEM";
                       xmlNode.attr["type"]  = "int";
                       xmlNode.attr["value"] = std::to_string(value.value);
                       if (auto v = generateRestrictions(value))
                           xmlNode.attr["restrictions"] = v;
                   },
                   [&](DoubleValue value) {
                       xmlNode.tag           = "ITEM";
                       xmlNode.attr["type"]  = "double";
                       xmlNode.attr["value"] = convertToString(value.value);
                       if (auto v = generateRestrictions(value))
                           xmlNode.attr["restrictions"] = v;
                   },
                   [&](StringValue value) {
                       xmlNode.tag = "ITEM";
                       if (tags.count("input file") > 0) {
                           tags.erase("input file");
                           xmlNode.attr["type"] = "input-file";
                           if (auto v = generateValidList(value))
                               xmlNode.attr["supported_formats"] = v;
                       } else if (tags.count("output file") > 0) {
                           tags.erase("output file");
                           xmlNode.attr["type"] = "output-file";
                           if (auto v = generateValidList(value))
                               xmlNode.attr["supported_formats"] = v;
                       } else if (tags.count("output prefix") > 0) {
                           tags.erase("output prefix");
                           xmlNode.attr["type"] = "output-prefix";
                           if (auto v = generateValidList(value))
                               xmlNode.attr["supported_formats"] = v;
                       } else {
                           xmlNode.attr["type"] = "string";
                           if (auto v = generateValidList(value))
                               xmlNode.attr["restrictions"] = v;
                       }
                       xmlNode.attr["value"] = value.value;
                   },
                   [&](IntValueList value) {
                       xmlNode.tag          = "ITEMLIST";
                       xmlNode.attr["type"] = "int";
                       for (auto const & v : value.value) {
                           xmlNode.children.push_back({"LISTITEM", {{"value", convertToString(v)}}});
                       }
                       if (auto v = generateRestrictions(value))
                           xmlNode.attr["restrictions"] = v;
                   },
                   [&](DoubleValueList value) {
                       xmlNode.tag          = "ITEMLIST";
                       xmlNode.attr["type"] = "double";
                       for (auto const & v : value.value) {
                           xmlNode.children.push_back({"LISTITEM", {{"value", convertToString(v)}}});
                       }
                       if (auto v = generateRestrictions(value))
                           xmlNode.attr["restrictions"] = v;
                   },
                   [&](StringValueList value) {
                       xmlNode.tag = "ITEMLIST";
                       if (tags.count("input file") > 0) {
                           tags.erase("input file");
                           xmlNode.attr["type"] = "input-file";
                           if (auto v = generateValidList(value))
                               xmlNode.attr["supported_formats"] = v;
                       } else if (tags.count("output file") > 0) {
                           tags.erase("output file");
                           xmlNode.attr["type"] = "output-file";
                           if (auto v = generateValidList(value))
                               xmlNode.attr["supported_formats"] = v;
                       } else {
                           xmlNode.attr["type"] = "string";
                           if (auto v = generateValidList(value))
                               xmlNode.attr["restrictions"] = v;
                       }
                       for (auto const & v : value.value) {
                           xmlNode.children.push_back({"LISTITEM", {{"value", v}}});
                       }
                   },
                   [&](Node::Children const & children) {
                       for (auto const & child : children) {
                           xmlNode.children.emplace_back(convertToCTD(child));
                       }
                   },
               },
               param.value);

    // add description if available
    if (!param.description.empty()) {
        xmlNode.attr["description"] = param.description;
    }

    if (xmlNode.tag != "NODE") {
        // convert special tags to special xml attributes
        auto convertTagToAttr = [&](std::string const & tag) {
            xmlNode.attr[tag] = tags.count(tag) ? "true" : "false";
            tags.erase(tag);
        };
        convertTagToAttr("required");
        convertTagToAttr("advanced");
    }

    // add all other tags to the "tags" xml attribute
    if (!tags.empty()) {
        xmlNode.attr["tags"] = std::accumulate(++begin(tags), end(tags), *begin(tags), [](auto acc, auto const & next) {
            acc.push_back(',');
            acc.append(next);
            return acc;
        });
    }

    return xmlNode;
}

} // namespace detail

/*!\brief converts a ToolInfo into a string that
 * holds the CTD representation of the given tool
 */
auto convertToCTD(ToolInfo const & doc) -> std::string {
    auto const & node      = doc.params;
    auto const & tool_info = doc.metaInfo;

    static auto const schema_location = std::string{"/SCHEMAS/Param_1_7_0.xsd"};
    static auto const schema_version  = std::string{"1.7.0"};

    // Write ctd specific stuff
    auto toolNode = detail::XMLNode{/*.tag = */ "tool",
                                    /*.attr = */ {{"ctdVersion", "1.7"}}};

    { // Add attributes
        auto emptyOrAttr = [&](std::string attr, std::string const & in) {
            if (!in.empty()) {
                toolNode.attr[attr] = in;
            }
        };
        emptyOrAttr("version", tool_info.version);
        emptyOrAttr("name", tool_info.name);
        emptyOrAttr("docurl", tool_info.docurl);
        emptyOrAttr("category", tool_info.category);
    }

    { // Add tags
        auto emptyOrTag = [&](std::string tag, std::string const & in) {
            if (!in.empty()) {
                toolNode.children.push_back(detail::XMLNode{/*.tag      =*/std::move(tag),
                                                            /*.attr     =*/{},
                                                            /*.children =*/{},
                                                            /*.data     =*/"<![CDATA[" + in + "]]>"});
            }
        };
        emptyOrTag("description", tool_info.description);
        emptyOrTag("manual", tool_info.description); //! TODO why does OpenMS do this?
        emptyOrTag("executableName", tool_info.executableName);
    }

    { // Add citations
        auto citationNode = detail::XMLNode{/*.tag = */ "citations"};
        for (auto const & [doi, url] : tool_info.citations) {
            citationNode.children.push_back({/*.tag = */ "citation",
                                             /*.attr = */ {{"doi", doi}, {"url", url}}});
        }
        toolNode.children.push_back(std::move(citationNode));
    }

    { // Add parameters
        auto paramsNode =
            detail::XMLNode{/*.tag = */ "PARAMETERS",
                            /*.attr = */
                            {{"version", schema_version},
                             {"xsi:noNamespaceSchemaLocation",
                              "https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS" + schema_location},
                             {"xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance"}}};

        for (auto const & child : node) {
            paramsNode.children.emplace_back(detail::convertToCTD(child));
        }
        toolNode.children.push_back(std::move(paramsNode));
    }

    // add cli mapping
    if (!doc.cliMapping.empty()) {
        auto cliNode = detail::XMLNode{/*.tag = */ "cli"};
        for (auto const & [optionIdentifier, referenceName] : doc.cliMapping) {
            cliNode.children.push_back(
                {/*.tag = */ "clielement", /*.attr = */ {{"optionIdentifier", optionIdentifier}}});
            cliNode.children.back().children.push_back(
                {/*.tag = */ "mapping", /*.attr = */ {{"referenceName", referenceName}}});
        }
        toolNode.children.emplace_back(std::move(cliNode));
    }

    auto ss = std::stringstream{};
    ss.precision(std::numeric_limits<double>::digits10);

    static auto order = std::vector<std::string>{"ctdVersion",
                                                 "version",
                                                 "name",
                                                 "value",
                                                 "type",
                                                 "description",
                                                 "docurl",
                                                 "category",
                                                 "xsi:noNamespaceSchemaLocation",
                                                 "xmlns:xsi",
                                                 "required",
                                                 "advanced",
                                                 "restrictions"};

    // Write ctd specific stuff
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    ss << toolNode.generateString(order);

    return ss.str();
}

} // namespace tdl
