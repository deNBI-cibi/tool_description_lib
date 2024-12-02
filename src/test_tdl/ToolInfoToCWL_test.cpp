// SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: CC0-1.0

#include "utils.h"

namespace cwl_tests {

void testToolInfo() {
    { // a multi nested parameter
        auto output = convertToCWL(tdl::ToolInfo{
            DESINIT(.metaInfo =){
                DESINIT(.version =){},
                DESINIT(.name =){},
                DESINIT(.docurl =){},
                DESINIT(.category =){},
                DESINIT(.description =){},
                DESINIT(.executableName =) "echo",
            },
            DESINIT(.params =){
                tdl::Node{DESINIT(.name =) "build",
                          DESINIT(.description =) "Command",
                          DESINIT(.tags =){"basecommand"}, // indicates this is an addition to the base command
                          DESINIT(.value =) tdl::Node::Children{tdl::Node{DESINIT(.name =) "kmer",
                                                                          DESINIT(.description =) "The k-mer size",
                                                                          DESINIT(.tags =){"required"},
                                                                          DESINIT(.value =) tdl::IntValue{20, 1, 32}},
                                                                tdl::Node{DESINIT(.name =) "window",
                                                                          DESINIT(.description =) "The window size",
                                                                          DESINIT(.tags =){},
                                                                          DESINIT(.value =) tdl::IntValue{22}}}}},
            DESINIT(.cliMapping =){
                {DESINIT(.optionIdentifier =) "--kmer", DESINIT(.referenceName =) "kmer"},
                {DESINIT(.optionIdentifier =) "--window", DESINIT(.referenceName =) "window"},
            },
        });

        auto expected = std::string{R"(label: ""
doc: ""
inputs:
  kmer:
    doc: The k-mer size
    type: long
    inputBinding:
      prefix: --kmer
  window:
    doc: The window size
    default: 22
    type: long?
    inputBinding:
      prefix: --window
outputs:
  []
cwlVersion: v1.2
class: CommandLineTool
baseCommand:
  - echo
  - build)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

void testComplexCall() {
    { // a multi nested parameter
        auto output = convertToCWL(tdl::ToolInfo{
            DESINIT(.metaInfo =){
                DESINIT(.version =){},
                DESINIT(.name =){},
                DESINIT(.docurl =){},
                DESINIT(.category =){},
                DESINIT(.description =){},
                DESINIT(.executableName =) "echo",
            },
            DESINIT(.params =){
                tdl::Node{DESINIT(.name =) "build",
                          DESINIT(.description =) "Command",
                          DESINIT(.tags =){"basecommand"},
                          DESINIT(.value =) tdl::Node::Children{
                              tdl::Node{DESINIT(.name =) "kmer",
                                        DESINIT(.description =) "The k-mer size",
                                        DESINIT(.tags =){"required"},
                                        DESINIT(.value =) tdl::IntValue{20, 1, 32}},
                              tdl::Node{DESINIT(.name =) "window",
                                        DESINIT(.description =) "The window size",
                                        DESINIT(.tags =){},
                                        DESINIT(.value =) tdl::IntValue{20}},
                              tdl::Node{DESINIT(.name =) "window_with_hint",
                                        DESINIT(.description =) "The window size that also gives a hint",
                                        DESINIT(.tags =){"required", "default_as_hint"},
                                        DESINIT(.value =) tdl::IntValue{20}},
                              tdl::Node{DESINIT(.name =) "single_input_file",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"input", "required", "file"},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "optional_single_input_file",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"input", "file"},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "multi_input_file",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"input", "required", "file"},
                                        DESINIT(.value =) tdl::StringValueList{}},
                              tdl::Node{DESINIT(.name =) "single_input_directory",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"input", "required", "directory"},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "single_output_file",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"output", "required", "file"},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "optional_single_output_file",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"output", "file"},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "prefixed_output_file",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"output", "prefixed", "required"},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "optional_prefixed_output_file",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"output", "prefixed"},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "prefixed_output_files",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"output", "prefixed", "required"},
                                        DESINIT(.value =) tdl::StringValueList{}},
                              tdl::Node{DESINIT(.name =) "optional_prefixed_output_files",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"output", "prefixed"},
                                        DESINIT(.value =) tdl::StringValueList{}},
                              tdl::Node{DESINIT(.name =) "single_output_dir",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"output", "required", "directory"},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "optional_param1",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "optional_param2_no_default",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"no_default"},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "optional_multi_input_file",
                                        DESINIT(.description =) "no doc",
                                        DESINIT(.tags =){"input", "file"},
                                        DESINIT(.value =) tdl::StringValueList{}},
                              tdl::Node{DESINIT(.name =) "no_cli_binding",
                                        DESINIT(.description =) "A parameter that has no corresponding cli binding."
                                                                " Can only be filled via an input.json file.",
                                        DESINIT(.tags =){},
                                        DESINIT(.value =) tdl::StringValue{}},
                              tdl::Node{DESINIT(.name =) "type",
                                        DESINIT(.description =) "A parameter that is called type.",
                                        DESINIT(.tags =){},
                                        DESINIT(.value =) tdl::StringValue{}},
                          }}},
            DESINIT(.cliMapping =){
                {DESINIT(.optionIdentifier =) "--kmer", DESINIT(.referenceName =) "kmer"},
                {DESINIT(.optionIdentifier =) "--window", DESINIT(.referenceName =) "window"},
                {DESINIT(.optionIdentifier =) "--window_with_hint", DESINIT(.referenceName =) "window_with_hint"},
                {DESINIT(.optionIdentifier =) "--single_input_file", DESINIT(.referenceName =) "single_input_file"},
                {DESINIT(.optionIdentifier =) "--optional_single_input_file",
                 DESINIT(.referenceName =) "optional_single_input_file"},
                {DESINIT(.optionIdentifier =) "--multi_input_file", DESINIT(.referenceName =) "multi_input_file"},
                {DESINIT(.optionIdentifier =) "--single_input_directory",
                 DESINIT(.referenceName =) "single_input_directory"},
                {DESINIT(.optionIdentifier =) "--single_output_file", DESINIT(.referenceName =) "single_output_file"},
                {DESINIT(.optionIdentifier =) "--optional_single_output_file",
                 DESINIT(.referenceName =) "optional_single_output_file"},
                {DESINIT(.optionIdentifier =) "--prefixed_output_file",
                 DESINIT(.referenceName =) "prefixed_output_file"},
                {DESINIT(.optionIdentifier =) "--optional_prefixed_output_file",
                 DESINIT(.referenceName =) "optional_prefixed_output_file"},
                {DESINIT(.optionIdentifier =) "--prefixed_output_files",
                 DESINIT(.referenceName =) "prefixed_output_files"},
                {DESINIT(.optionIdentifier =) "--optional_prefixed_output_files",
                 DESINIT(.referenceName =) "optional_prefixed_output_files"},
                {DESINIT(.optionIdentifier =) "--single_output_dir", DESINIT(.referenceName =) "single_output_dir"},
                {DESINIT(.optionIdentifier =) "--optional_param1", DESINIT(.referenceName =) "optional_param1"},
                {DESINIT(.optionIdentifier =) "--optional_param2_no_default", DESINIT(.referenceName =) "optional_param2_no_default"},
                {DESINIT(.optionIdentifier =) "--optional_multi_input_file",
                 DESINIT(.referenceName =) "optional_multi_input_file"},
            },
        });

        auto expected = std::string{R"(label: ""
doc: ""
inputs:
  kmer:
    doc: The k-mer size
    type: long
    inputBinding:
      prefix: --kmer
  window:
    doc: The window size
    default: 20
    type: long?
    inputBinding:
      prefix: --window
  window_with_hint:
    doc: The window size that also gives a hint
    default: 20
    type: long
    inputBinding:
      prefix: --window_with_hint
  single_input_file:
    doc: no doc
    type: File
    inputBinding:
      prefix: --single_input_file
  optional_single_input_file:
    doc: no doc
    type: File?
    inputBinding:
      prefix: --optional_single_input_file
  multi_input_file:
    doc: no doc
    type: File[]
    inputBinding:
      prefix: --multi_input_file
  single_input_directory:
    doc: no doc
    type: Directory
    inputBinding:
      prefix: --single_input_directory
  single_output_file:
    doc: no doc
    type: string
    inputBinding:
      prefix: --single_output_file
  optional_single_output_file:
    doc: no doc
    type: string?
    inputBinding:
      prefix: --optional_single_output_file
  prefixed_output_file:
    doc: no doc
    type: string
    inputBinding:
      prefix: --prefixed_output_file
  optional_prefixed_output_file:
    doc: no doc
    type: string?
    inputBinding:
      prefix: --optional_prefixed_output_file
  prefixed_output_files:
    doc: no doc
    type: string
    inputBinding:
      prefix: --prefixed_output_files
  optional_prefixed_output_files:
    doc: no doc
    type: string?
    inputBinding:
      prefix: --optional_prefixed_output_files
  single_output_dir:
    doc: no doc
    type: string
    inputBinding:
      prefix: --single_output_dir
  optional_param1:
    doc: no doc
    default: ""
    type: string?
    inputBinding:
      prefix: --optional_param1
  optional_param2_no_default:
    doc: no doc
    type: string?
    inputBinding:
      prefix: --optional_param2_no_default
  optional_multi_input_file:
    doc: no doc
    type: File[]?
    inputBinding:
      prefix: --optional_multi_input_file
  no_cli_binding:
    doc: A parameter that has no corresponding cli binding. Can only be filled via an input.json file.
    default: ""
    type: string?
  type:
    doc: A parameter that is called type.
    default: ""
    type: string?
outputs:
  single_output_file:
    type: File
    outputBinding:
      glob: $(inputs.single_output_file)
  optional_single_output_file:
    type: File?
    outputBinding:
      glob: $(inputs.optional_single_output_file)
  prefixed_output_file:
    type: File
    outputBinding:
      glob: $(inputs.prefixed_output_file)*
  optional_prefixed_output_file:
    type: File?
    outputBinding:
      glob: $(inputs.optional_prefixed_output_file)*
  prefixed_output_files:
    type: File[]
    outputBinding:
      glob: $(inputs.prefixed_output_files)*
  optional_prefixed_output_files:
    type: File[]?
    outputBinding:
      glob: $(inputs.optional_prefixed_output_files)*
  single_output_dir:
    type: Directory
    outputBinding:
      glob: $(inputs.single_output_dir)
cwlVersion: v1.2
class: CommandLineTool
baseCommand:
  - echo
  - build)"};
        assert(output.size() == expected.size());
        assert(expected == output);
    }
}

} // namespace cwl_tests
void testCWL() {
    cwl_tests::testToolInfo();
    cwl_tests::testComplexCall();
}
