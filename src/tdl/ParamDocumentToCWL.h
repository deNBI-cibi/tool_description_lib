// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2023, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2023, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file.
// -----------------------------------------------------------------------------------------------------

#pragma once

#include "ParamDocument.h"
#include "generated-cwl.h"

#include <cassert>
#include <filesystem>
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

//!\brief overload structure allowing fancy 'std::visit` syntax
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

//!\brief required deduction guide for c++17 (not required for c++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace cwl = https___w3id_org_cwl_cwl;

/**!\brief a global callback function to adjust the exporting for cwl
 *
 * This callback allows to adjust the exported yaml file to add/change/remove
 * cwl entries, which currently arn't controllable via tdl itself.
 */
inline std::function<void(YAML::Node&)> post_process_cwl;

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
    tool.label       = tool_info.name;
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
    baseCommand.push_back(std::filesystem::path{tool_info.executableName}.filename().string());

    // add cli mapping
    for (auto const& [optionIdentifier, referenceName] : doc.cliMapping) {
        auto ptr = findNode(referenceName);

        auto addInput = [&, &referenceName=referenceName, &optionIdentifier=optionIdentifier](auto type) {
            auto input         = cwl::CommandInputParameter{};
            input.id           = referenceName;
            if (ptr->tags.count("required")) {
                input.type         = type;
            } else {
                using namespace https___w3id_org_cwl_cwl;
                input.type         = std::vector<std::variant<CWLType, CommandInputRecordSchema, CommandInputEnumSchema, CommandInputArraySchema, std::string>>{cwl::CWLType::null, type};
            }
            input.doc          = ptr->description;

            auto binding       = cwl::CommandLineBinding{};
            binding.prefix     = optionIdentifier;
            input.inputBinding = binding;
            tool.inputs->push_back(input);
        };

        auto addOutput = [&, &referenceName=referenceName, &optionIdentifier=optionIdentifier](auto type) {
            auto input         = cwl::CommandInputParameter{};
            input.id           = referenceName;
            input.type         = cwl::CWLType::string;
            input.doc          = ptr->description;

            auto binding       = cwl::CommandLineBinding{};
            binding.prefix     = optionIdentifier;
            input.inputBinding = binding;
            tool.inputs->push_back(input);

            auto output         = cwl::CommandOutputParameter{};
            output.id           = referenceName;
            output.type         = type;

            auto binding2       = cwl::CommandOutputBinding{};
            binding2.glob       = "$(inputs." + referenceName + ")";
            output.outputBinding = binding2;
            tool.outputs->push_back(output);
        };



        if (ptr) {
            std::visit(overloaded{
                [&](BoolValue const&) {
                    addInput(cwl::CWLType::boolean);
                },
                [&](IntValue const&) {
                    addInput(cwl::CWLType::long_);
                },
                [&](DoubleValue const&) {
                    addInput(cwl::CWLType::double_);
                },
                [&, &optionIdentifier=optionIdentifier](StringValue const& v) {
                    if (optionIdentifier.empty()) {
                        baseCommand.push_back(v.value);
                        return;
                    }

                    if (ptr->tags.count("output")) {
                        if (ptr->tags.count("file")) {
                            addOutput(cwl::CWLType::File);
                        } else if (ptr->tags.count("directory")) {
                            addOutput(cwl::CWLType::Directory);
                        }
                    } else if (ptr->tags.count("file")) {
                        addInput(cwl::CWLType::File);
                    } else if (ptr->tags.count("directory")) {
                        addInput(cwl::CWLType::Directory);
                    } else {
                        addInput(cwl::CWLType::string);
                    }
                },
                [&](auto const&) {
                    addInput(cwl::CWLType::null);
                },
            }, ptr->value);
        }
    }
    tool.baseCommand = baseCommand;


    auto y = toYaml(tool);

    // post process generated cwl yaml file
    if (post_process_cwl) {
        post_process_cwl(y);
    }

    YAML::Emitter out;
    out << y;
    return out.c_str();
}

}

#if defined(_WIN32) || defined(WIN32)
#undef and
#undef or
#undef not
#endif
