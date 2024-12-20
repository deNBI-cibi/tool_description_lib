// SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: BSD-3-Clause

#include "convertToCWL.h"

#include <cassert>
#include <filesystem>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>

#include "cwl_v1_2.h"

namespace tdl {

namespace cwl = w3id_org::cwl::cwl;

//!\brief overload structure allowing fancy 'std::visit` syntax
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

//!\brief required deduction guide for c++17 (not required for c++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace detail {

//!\brief convenience function, to have highest precision for float/double values
template <typename T>
auto convertToString(T v) -> std::string {
    auto ss = std::stringstream{};
    ss << std::setprecision(std::numeric_limits<std::decay_t<T>>::max_digits10) << v;
    return ss.str();
}


inline auto simplifyType(YAML::Node type) -> YAML::Node {
    auto is_optional = [](YAML::Node const & node) {
        return node.IsSequence() && node.size() == 2u && node[0].Scalar() == "null";
    };

    auto is_array = [](YAML::Node const & node) {
        return node.IsMap() && node["type"].Scalar() == "array" && node["items"].IsScalar();
    };

    // 1. Collapsing optional scalar types into one option
    if (is_optional(type) && type[1].IsScalar()) {
        type = type[1].as<std::string>() + "?";
    }

    // 2. Collapsing array types into one option
    if (is_array(type)) {
        type = type["items"].as<std::string>() + "[]";
    }

    // 3. Collapsing optional array types into one option
    if (is_optional(type) && is_array(type[1])) {
        type = type[1]["items"].as<std::string>() + "[]?";
    }

    return type;
}

inline auto findCLIMapping(std::string const & referenceName, ToolInfo const & doc) -> CLIMapping const * {
    for (auto const & mapping : doc.cliMapping) {
        if (mapping.referenceName == referenceName) {
            return &mapping;
        }
    }
    return nullptr;
}

template <typename InputType>
void setIdOrName(InputType & input, std::string const & name) {
    if constexpr (std::is_same_v<InputType, cwl::CommandInputRecordField>
                  || std::is_same_v<InputType, cwl::CommandOutputRecordField>) {
        input.name = name;
    } else {
        input.id = name;
    }
}

template <typename TypeType, typename InputType, typename InputCB>
void addInput_impl(TypeType const &   type,
                   InputType          input,
                   Node const &       child,
                   InputCB const &    inputCB,
                   CLIMapping const * cliMapping) {
    setIdOrName(input, child.name);

    if (child.tags.count("required")) {
        input.type = type;
    } else {
        input.type = std::vector<std::variant<cwl::CWLType,
                                              cwl::CommandInputRecordSchema,
                                              cwl::CommandInputEnumSchema,
                                              cwl::CommandInputArraySchema,
                                              std::string>>{cwl::CWLType::null, type};
    }
    if constexpr (std::is_same_v<InputType, cwl::CommandInputParameter>) {
        if ((child.tags.count("required") == 0 && child.tags.count("no_default") == 0)
            || (child.tags.count("required") && child.tags.count("default_as_hint"))) {
            // produce a default value
            std::visit(overloaded{
                           [&](BoolValue const & v) {
                               *input.default_ = v;
                           },
                           [&](IntValue const & v) {
                               *input.default_ = v.value;
                           },
                           [&](DoubleValue const & v) {
                               *input.default_ = v.value;
                           },
                           [&](StringValue const & v) {
                               if (!child.tags.count("output")
                                    && !child.tags.count("file")
                                    && !child.tags.count("directory")
                                    && !child.tags.count("prefixed")) {

                                   *input.default_ = v.value;
                                } else {
                                    *input.default_ = "unsupported default";
                                }
                           },
                           [&](auto const&) {}
            }, child.value);
        }
    }

    input.doc = child.description;

    if (cliMapping) {
        auto binding       = cwl::CommandLineBinding{};
        binding.prefix     = cliMapping->optionIdentifier;
        input.inputBinding = binding;
    }

    inputCB(std::move(input));
}

template <typename TypeType, typename InputType, typename InputCB>
void addInputArray_impl(TypeType const &   type,
                        InputType          input,
                        Node const &       child,
                        InputCB const &    inputCB,
                        CLIMapping const * cliMapping) {
    setIdOrName(input, child.name);

    auto arrayType  = cwl::CommandInputArraySchema{};
    arrayType.items = type;

    if (child.tags.count("required")) {
        input.type = arrayType;
    } else {
        input.type = std::vector<std::variant<cwl::CWLType,
                                              cwl::CommandInputRecordSchema,
                                              cwl::CommandInputEnumSchema,
                                              cwl::CommandInputArraySchema,
                                              std::string>>{cwl::CWLType::null, arrayType};
    }

    input.doc = child.description;

    if (cliMapping) {
        auto binding       = cwl::CommandLineBinding{};
        binding.prefix     = cliMapping->optionIdentifier;
        input.inputBinding = binding;
    }

    inputCB(std::move(input));
}

template <typename TypeType, typename InputType, typename InputCB, typename OutputType, typename OutputCB>
void addOutput_impl(TypeType const &   type,
                    InputType          input,
                    Node const &       child,
                    InputCB const &    inputCB,
                    CLIMapping const * cliMapping,
                    OutputType         output,
                    OutputCB const &   outputCB) {
    setIdOrName(input, child.name);

    if (child.tags.count("required")) {
        input.type = cwl::CWLType::string;
    } else {
        input.type = std::vector<std::variant<cwl::CWLType,
                                              cwl::CommandInputRecordSchema,
                                              cwl::CommandInputEnumSchema,
                                              cwl::CommandInputArraySchema,
                                              std::string>>{cwl::CWLType::null, cwl::CWLType::string};
    }

    input.doc = child.description;

    if (cliMapping) {
        auto binding       = cwl::CommandLineBinding{};
        binding.prefix     = cliMapping->optionIdentifier;
        input.inputBinding = binding;
    }

    inputCB(std::move(input));

    setIdOrName(output, child.name);

    if (child.tags.count("required")) {
        output.type = type;
    } else {
        output.type = std::vector<std::variant<cwl::CWLType,
                                               cwl::CommandOutputRecordSchema,
                                               cwl::CommandOutputEnumSchema,
                                               cwl::CommandOutputArraySchema,
                                               std::string>>{cwl::CWLType::null, type};
    }

    auto binding         = cwl::CommandOutputBinding{};
    binding.glob         = "$(inputs." + child.name + ")";
    output.outputBinding = binding;

    outputCB(std::move(output));
}

template <typename TypeType, typename InputType, typename InputCB, typename OutputType, typename OutputCB>
void addOutputPrefixed_impl(TypeType const &   type,
                            InputType          input,
                            Node const &       child,
                            InputCB const &    inputCB,
                            CLIMapping const * cliMapping,
                            OutputType         output,
                            OutputCB const &   outputCB,
                            bool               multipleFiles) {
    setIdOrName(input, child.name);

    if (child.tags.count("required")) {
        input.type = cwl::CWLType::string;
    } else {
        input.type = std::vector<std::variant<cwl::CWLType,
                                              cwl::CommandInputRecordSchema,
                                              cwl::CommandInputEnumSchema,
                                              cwl::CommandInputArraySchema,
                                              std::string>>{cwl::CWLType::null, cwl::CWLType::string};
    }

    input.doc = child.description;

    if (cliMapping) {
        auto binding       = cwl::CommandLineBinding{};
        binding.prefix     = cliMapping->optionIdentifier;
        input.inputBinding = binding;
    }

    inputCB(std::move(input));

    setIdOrName(output, child.name);

    // Setting the correct value type
    output.type = type;
    // Add an array, if a list of files
    if (multipleFiles) {
        auto arrayType  = cwl::CommandOutputArraySchema{};
        arrayType.items = type;
        output.type     = arrayType;
    }

    // Add a null, if not required and an array, if a list of files
    if (!child.tags.count("required")) {
        output.type = std::vector<std::variant<cwl::CWLType,
                                               cwl::CommandOutputRecordSchema,
                                               cwl::CommandOutputEnumSchema,
                                               cwl::CommandOutputArraySchema,
                                               std::string>>{cwl::CWLType::null, type};

        if (multipleFiles) {
            auto arrayType  = cwl::CommandOutputArraySchema{};
            arrayType.items = type;
            output.type     = std::vector<std::variant<cwl::CWLType,
                                                   cwl::CommandOutputRecordSchema,
                                                   cwl::CommandOutputEnumSchema,
                                                   cwl::CommandOutputArraySchema,
                                                   std::string>>{cwl::CWLType::null, arrayType};
        }
    }

    auto binding         = cwl::CommandOutputBinding{};
    binding.glob         = "$(inputs." + child.name + ")*";
    output.outputBinding = binding;
    outputCB(std::move(output));
}

//!\brief Recursive function that traverses the tree of the tool and generates the CWL representation.
// CB = Callback
template <typename InputType  = cwl::CommandInputParameter,
          typename OutputType = cwl::CommandOutputParameter,
          size_t deep         = 5,
          typename InputCB,
          typename OutputCB,
          typename BaseCommandCB>
inline void generateCWL(Node::Children const & children,
                        ToolInfo const &       doc,
                        InputCB const &        inputCB,
                        OutputCB const &       outputCB,
                        BaseCommandCB const &  baseCommandCB) {
    if constexpr (deep > 0) {
        for (auto child : children) {
            auto cliMapping = findCLIMapping(child.name, doc);

            auto addInput = [&](auto type) {
                addInput_impl(type, InputType{}, child, inputCB, cliMapping);
            };

            auto addInputArray = [&](auto type) {
                addInputArray_impl(type, InputType{}, child, inputCB, cliMapping);
            };

            auto addOutput = [&](auto type) {
                addOutput_impl(type, InputType{}, child, inputCB, cliMapping, OutputType{}, outputCB);
            };

            auto addOutputPrefixed = [&](auto type, bool multipleFiles) {
                addOutputPrefixed_impl(type,
                                       InputType{},
                                       child,
                                       inputCB,
                                       cliMapping,
                                       OutputType{},
                                       outputCB,
                                       multipleFiles);
            };

            std::visit(overloaded{
                           [&](BoolValue const &) {
                               addInput(cwl::CWLType::boolean);
                           },
                           [&](IntValue const &) {
                               addInput(cwl::CWLType::long_);
                           },
                           [&](DoubleValue const &) {
                               addInput(cwl::CWLType::double_);
                           },
                           [&](StringValue const &) {
                               if (child.tags.count("output")) {
                                   if (child.tags.count("file")) {
                                       addOutput(cwl::CWLType::File);
                                   } else if (child.tags.count("directory")) {
                                       addOutput(cwl::CWLType::Directory);
                                   } else if (child.tags.count("prefixed")) {
                                       addOutputPrefixed(cwl::CWLType::File, /*.mutliplieFiles = */ false);
                                   }
                               } else if (child.tags.count("file")) {
                                   addInput(cwl::CWLType::File);
                               } else if (child.tags.count("directory")) {
                                   addInput(cwl::CWLType::Directory);
                               } else {
                                   addInput(cwl::CWLType::string);
                               }
                           },
                           [&](IntValueList const &) {
                               addInputArray(cwl::CWLType::long_);
                           },
                           [&](DoubleValueList const &) {
                               addInputArray(cwl::CWLType::double_);
                           },
                           [&](StringValueList const &) {
                               if (child.tags.count("output")) {
                                   if (child.tags.count("prefixed")) {
                                       addOutputPrefixed(cwl::CWLType::File, /*.multipleFiles =*/true);
                                   } else {
                                       //! TODO not implemented
                                   }
                               } else if (child.tags.count("file")) {
                                   addInputArray(cwl::CWLType::File);
                               } else if (child.tags.count("directory")) {
                                   addInputArray(cwl::CWLType::Directory);
                               } else {
                                   addInputArray(cwl::CWLType::string);
                               }
                           },
                           [&](Node::Children const & recursive_children) {
                               if (child.tags.count("basecommand")) {
                                   baseCommandCB(child.name);

                                   generateCWL<InputType, OutputType, deep - 1>(recursive_children,
                                                                                doc,
                                                                                inputCB,
                                                                                outputCB,
                                                                                baseCommandCB);
                                   return;
                               }

                               auto inputs  = std::vector<cwl::CommandInputRecordField>{};
                               auto outputs = std::vector<cwl::CommandOutputRecordField>{};

                               generateCWL<cwl::CommandInputRecordField, cwl::CommandOutputRecordField, (deep - 1)>(
                                   recursive_children,
                                   doc,
                                   [&](auto input) {
                                       inputs.push_back(std::move(input));
                                   },
                                   [&](auto output) {
                                       outputs.push_back(std::move(output));
                                   },
                                   baseCommandCB);

                               auto inputType = cwl::CommandInputRecordSchema{};

                               inputType.fields = inputs;
                               addInput(inputType);
                           },
                       },
                       child.value);
        }
    }
}

} // namespace detail

auto tool_to_yaml(ToolInfo const & doc) -> YAML::Node {
    auto & tool_info = doc.metaInfo;

    auto tool       = cwl::CommandLineTool{};
    tool.cwlVersion = cwl::CWLVersion::v1_2;
    tool.label      = tool_info.name;
    tool.doc        = tool_info.description;
    // = tool_info.category; //!TODO implement this feature
    // = tool_info.docurl; //!TODO implement this feature
    // = tool_info.version; //!TODO implement this feature

    //! TODO implement export of citation information
    // for (auto& [doi, url] : tool_info.citations) {
    //     citationNode.children.push_back({/*.tag = */"citation",
    //                                      /*.attr = */{{"doi", doi}, {"url", url}}});
    // }
    // toolNode.children.push_back(citationNode);

    auto baseCommand = std::vector<std::string>{};
    baseCommand.push_back(std::filesystem::path{tool_info.executableName}.filename().string());

    detail::generateCWL(
        doc.params,
        doc,
        [&](auto input) {
            tool.inputs->push_back(std::move(input));
        },
        [&](auto output) {
            tool.outputs->push_back(std::move(output));
        },
        [&](auto command) {
            baseCommand.push_back(std::move(command));
        });

    tool.baseCommand = std::move(baseCommand);

    return w3id_org::cwl::toYaml(tool, {});
}

/*!\brief converts a ToolInfo into a string that
 * holds the CWL representation of the given tool
 */
auto convertToCWL(ToolInfo const & doc) -> std::string {
    auto yaml = tool_to_yaml(doc);

    // function to traverse yaml tree and executes 'simplifyType' on all nodes with name 'type'
    auto traverseTree = std::function<void(YAML::Node)>{};
    traverseTree      = [&traverseTree](YAML::Node node) {
        if (node.IsMap()) {
            for (auto n : node) {
                if (n.first.Scalar() == "type") {
                    n.second = detail::simplifyType(n.second);
                }

                traverseTree(n.second);
            }
        } else if (node.IsSequence()) {
            for (auto n : node) {
                traverseTree(n);
            }
        }
    };

    // Post procssing inputs and outputs of the yaml object
    for (auto param : {"inputs", "outputs"}) {
        traverseTree(yaml[param]);
    }

    // post process generated cwl yaml file
    if (post_process_cwl) {
        post_process_cwl(yaml);
    }

    YAML::Emitter out;
    out << yaml;
    return out.c_str();
}

} // namespace tdl
