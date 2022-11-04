// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file.
// -----------------------------------------------------------------------------------------------------

#pragma once

#include "ParamDocument.h"
#include "generated-cwl.h"

#include <cassert>
#include <functional>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>


#if defined(_WIN32) || defined(WIN32)
#   // Maybe visual studio will get there one day to support the c++ standard...
#   // Until then we have to live with this:
#   define and &&
#   define or ||
#   define not !
#endif


namespace tdl {

#if 0
namespace detail {
//!\brief converts a Node into an XMLNode
inline auto convertToCWL(Node const& param) -> XMLNode {
    // generate a single Node
    auto xmlNode = XMLNode{/*.tag = */"NODE",
                           /*.attr = */{{"name", param.name}, {"description", param.description}}};


    auto tags = param.tags; // copy tags to make them mutable

    // fill xmlNode depending on the actuall type
    std::visit(overloaded{
        [&](BoolValue value) {
            xmlNode.tag = "ITEM";
            xmlNode.attr["type"] = "bool";
            xmlNode.attr["value"] = value?"true":"false";
        },
        [&](IntValue value) {
            xmlNode.tag = "ITEM";
            xmlNode.attr["type"]  = "int";
            xmlNode.attr["value"] = std::to_string(value.value);
            if (auto v = generateRestrictions(value)) xmlNode.attr["restrictions"] = v;
        },
        [&](DoubleValue value) {
            xmlNode.tag = "ITEM";
            xmlNode.attr["type"] = "double";
            xmlNode.attr["value"] = convertToString(value.value);
            if (auto v = generateRestrictions(value)) xmlNode.attr["restrictions"] = v;
        },
        [&](StringValue value) {
            xmlNode.tag = "ITEM";
            if (tags.count("input file") > 0) {
                tags.erase("input file");
                xmlNode.attr["type"] = "input-file";
                if (auto v = generateValidList(value)) xmlNode.attr["supported_formats"] = v;
            } else if (tags.count("output file") > 0) {
                tags.erase("output file");
                xmlNode.attr["type"] = "output-file";
                if (auto v = generateValidList(value)) xmlNode.attr["supported_formats"] = v;
            } else if (tags.count("output prefix") > 0) {
                tags.erase("output prefix");
                xmlNode.attr["type"] = "output-prefix";
                if (auto v = generateValidList(value)) xmlNode.attr["supported_formats"] = v;
            } else {
                xmlNode.attr["type"] = "string";
                if (auto v = generateValidList(value)) xmlNode.attr["restrictions"] = v;
            }
            xmlNode.attr["value"] = value.value;
        },
        [&](IntValueList value) {
            xmlNode.tag = "ITEMLIST";
            xmlNode.attr["type"] = "int";
            for (auto const& v : value.value) {
                xmlNode.children.push_back({"LISTITEM", {{"value", convertToString(v)}}});
            }
            if (auto v = generateRestrictions(value)) xmlNode.attr["restrictions"] = v;

        },
        [&](DoubleValueList value) {
            xmlNode.tag = "ITEMLIST";
            xmlNode.attr["type"] = "double";
            for (auto const& v : value.value) {
                xmlNode.children.push_back({"LISTITEM", {{"value", convertToString(v)}}});
            }
            if (auto v = generateRestrictions(value)) xmlNode.attr["restrictions"] = v;
        },
        [&](StringValueList value) {
            xmlNode.tag = "ITEMLIST";
            if (tags.count("input file") > 0) {
                tags.erase("input file");
                xmlNode.attr["type"] = "input-file";
                if (auto v = generateValidList(value)) xmlNode.attr["supported_formats"] = v;
            } else if (tags.count("output file") > 0) {
                tags.erase("output file");
                xmlNode.attr["type"] = "output-file";
                if (auto v = generateValidList(value)) xmlNode.attr["supported_formats"] = v;
            } else {
                xmlNode.attr["type"] = "string";
                if (auto v = generateValidList(value)) xmlNode.attr["restrictions"] = v;
            }
            for (auto const& v : value.value) {
                xmlNode.children.push_back({"LISTITEM", {{"value", v}}});
            }
        },
        [&](Node::Children const& children) {
            for (auto const& child : children) {
                xmlNode.children.emplace_back(convertToCTD(child));
            }
        },
    }, param.value);

    // add description if available
    if (not param.description.empty()) {
        xmlNode.attr["description"] = param.description;
    }

    if (xmlNode.tag != "NODE") {
        // convert special tags to special xml attributes
        auto convertTagToAttr = [&](std::string const& tag) {
            xmlNode.attr[tag] = tags.count(tag)?"true":"false";
            tags.erase(tag);
        };
        convertTagToAttr("required");
        convertTagToAttr("advanced");
    }

    // add all other tags to the "tags" xml attribute
    if (not tags.empty()) {
        auto list = std::accumulate(++begin(tags), end(tags), *begin(tags), [](auto acc, auto const& next) {
            acc.append(",");
            acc.append(next);
            return acc;
        });
        xmlNode.attr["tags"] = list;
    }
    return xmlNode;
}

}
#endif

//!\brief overload structure allowing fancy 'std::visit` syntax
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

//!\brief required deduction guide for c++17 (not required for c++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;



namespace cwl = https___w3id_org_cwl_cwl;

/*!\brief converts a ToolInfo into a string that
 * holds the CWL representation of the given tool
 */
inline auto convertToCWL(ToolInfo const& doc) -> std::string {
    auto& nodes = doc.params;
    auto& tool_info = doc.metaInfo;
    auto const schema_location = std::string{"/SCHEMAS/Param_1_7_0.xsd"};
    auto const schema_version  = std::string{"1.7.0"};


    auto tool = cwl::CommandLineTool{};
    tool.cwlVersion  = cwl::CWLVersion::v1_2;
    tool.id          = tool_info.name;
    tool.label       = tool_info.name; //!TODO Label and id are the same?
    tool.doc         = tool_info.description;
    // = tool_info.category; //!TODO
    // = tool_info.docurl; //!TODO
    // = tool_info.version; //!TODO


    //!TODO Add citation information
    // for (auto& [doi, url] : tool_info.citations) {
    //     citationNode.children.push_back({/*.tag = */"citation",
    //                                      /*.attr = */{{"doi", doi}, {"url", url}}});
    // }
    // toolNode.children.push_back(citationNode);
    auto findNodeImpl = std::function<Node const*(std::string const&, Node const&)>{};
    findNodeImpl = [&](std::string const& key, Node const& next) -> Node const* {
        if (next.name == key) {
            return &next;
        }
        if (auto const* ptr = std::get_if<std::vector<Node>>(&next.value)) {
            for (auto const& n : *ptr) {
                auto r = findNodeImpl(key, n);
                if (r) {
                    return r;
                }
            }
        }
        return nullptr;
    };
    auto findNode = [&](std::string const& key) -> Node const* {
        for (auto const& n : nodes) {
            auto r = findNodeImpl(key, n);
            if (r) {
                return r;
            }
        }
        return nullptr;
    };

    auto baseCommand = std::vector<std::string>{};
    baseCommand.push_back(tool_info.executableName);

    // add cli mapping
    if (not doc.cliMapping.empty()) {
        for (auto const& [optionIdentifier, referenceName] : doc.cliMapping) {
            auto input         = cwl::CommandInputParameter{};
            input.id           = referenceName;

            auto ptr = findNode(referenceName);
            if (ptr) {
                std::visit(overloaded{
                    [&](BoolValue const&) {
                        input.type = cwl::CWLType::boolean;
                    },
                    [&](IntValue const&) {
                        input.type = cwl::CWLType::long_;
                    },
                    [&](DoubleValue const&) {
                        input.type = cwl::CWLType::double_;
                    },
                    [&](StringValue const& v) {
                        if (optionIdentifier.empty()) {
                            baseCommand.push_back(v.value);
                        }
                        input.type = cwl::CWLType::string;
                    },
                    [&](auto const&) {
                        input.type = cwl::CWLType::null;
                    },
                }, ptr->value);
            }

            if (optionIdentifier.empty()) {
                continue;
            }

            auto binding       = cwl::CommandLineBinding{};
            binding.prefix     = optionIdentifier;
            input.inputBinding = binding;
            tool.inputs->push_back(input);
        }
    }
        tool.baseCommand = baseCommand;


#if 0

    {
        auto input = cwl::CommandInputParameter{};
        *input.id = "first";
        auto record = cwl::CommandInputRecordSchema{};

        auto fieldEntry = cwl::CommandInputRecordField{};
        *fieldEntry.name = "species";

        auto species = cwl::CommandInputEnumSchema{};
        species.symbols->push_back("homo_sapiens");
        species.symbols->push_back("mus_musculus");

        using ListType = std::vector<std::variant<cwl::CWLType, cwl::CommandInputRecordSchema, cwl::CommandInputEnumSchema, cwl::CommandInputArraySchema, std::string>>;
        *fieldEntry.type = ListType{species, "null"};

        using ListType2 = std::vector<cwl::CommandInputRecordField>;
        *record.fields = ListType2{fieldEntry};

        using ListType3 = std::vector<std::variant<cwl::CWLType, cwl::CommandInputRecordSchema, cwl::CommandInputEnumSchema, cwl::CommandInputArraySchema, std::string>>;
        *input.type = ListType3{record};

        tool.inputs->push_back(input);
    }
#endif

    auto y = toYaml(tool);

    YAML::Emitter out;
    out << y;
    return out.c_str();
#if 0


    auto paramsNode = detail::XMLNode{/*.tag = */"PARAMETERS",
                                      /*.attr = */{{"version", schema_version},
                                               {"xsi:noNamespaceSchemaLocation", "https://raw.githubusercontent.com/OpenMS/OpenMS/develop/share/OpenMS" + schema_location},
                                               {"xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance"}}};

    for (auto child : node) {
        paramsNode.children.emplace_back(detail::convertToCTD(child));
    }
    toolNode.children.push_back(paramsNode);

    // add cli mapping
    if (not doc.cliMapping.empty()) {
        auto cliNode = detail::XMLNode{/*.tag = */"cli"};
        for (auto const& [optionIdentifier, referenceName] : doc.cliMapping) {
            cliNode.children.push_back({/*.tag = */"clielement", /*.attr = */{{"optionIdentifier", optionIdentifier}}});
            cliNode.children.back().children.push_back({/*.tag = */"mapping", /*.attr = */{{"referenceName", referenceName}}});
        }
        toolNode.children.emplace_back(std::move(cliNode));
    }


    auto ss = std::stringstream{};
    ss.precision(std::numeric_limits<double>::digits10);

    auto order = std::vector<std::string>{"ctdVersion", "version", "name", "value", "type", "description", "docurl", "category", "xsi:noNamespaceSchemaLocation", "xmlns:xsi", "required", "advanced", "restrictions"};

    // Write ctd specific stuff
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    ss << toolNode.generateString(order);

    return ss.str();
#endif

}

}

#if defined(_WIN32) || defined(WIN32)
#undef and
#undef or
#undef not
#endif
