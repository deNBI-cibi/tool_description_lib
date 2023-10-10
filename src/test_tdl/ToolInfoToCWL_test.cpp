// SPDX-FileCopyrightText: 2006-2023, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2023, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: CC0-1.0

#include "utils.h"


namespace cwl_tests {

void testToolInfo() {
    { // a multi nested parameter
        auto output = convertToCWL(tdl::ToolInfo {
            CPP20(.metaInfo =) {
                CPP20(.version        =) {},
                CPP20(.name           =) {},
                CPP20(.docurl         =) {},
                CPP20(.category       =) {},
                CPP20(.description    =) {},
                CPP20(.executableName =) "echo",
            },
            CPP20(.params =) {
                tdl::Node{CPP20(.name        =) "build",
                          CPP20(.description =) "Command",
                          CPP20(.tags        =) {"basecommand"}, // indicates this is an addition to the base command
                          CPP20(.value       =) tdl::Node::Children {
                                tdl::Node{CPP20(.name        =) "kmer",
                                            CPP20(.description =) "The k-mer size",
                                            CPP20(.tags        =) {"required"},
                                            CPP20(.value       =) tdl::IntValue{20, 1, 32}
                                },
                                tdl::Node{CPP20(.name        =) "window",
                                            CPP20(.description =) "The window size",
                                            CPP20(.tags        =) {"required"},
                                            CPP20(.value       =) tdl::IntValue{20}
                               }
                          }
                }
            },
            CPP20(.cliMapping =) {
                {CPP20(.optionIdentifier =) "--kmer",   CPP20(.referenceName    =) "kmer"},
                {CPP20(.optionIdentifier =) "--window", CPP20(.referenceName    =) "window"},
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
    type: long
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
        auto output = convertToCWL(tdl::ToolInfo {
            CPP20(.metaInfo =) {
                CPP20(.version        =) {},
                CPP20(.name           =) {},
                CPP20(.docurl         =) {},
                CPP20(.category       =) {},
                CPP20(.description    =) {},
                CPP20(.executableName =) "echo",
            },
            CPP20(.params =) {
                tdl::Node{CPP20(.name        =) "build",
                          CPP20(.description =) "Command",
                          CPP20(.tags        =) {"basecommand"},
                          CPP20(.value       =) tdl::Node::Children {
                                tdl::Node{CPP20(.name        =) "kmer",
                                            CPP20(.description =) "The k-mer size",
                                            CPP20(.tags        =) {"required"},
                                            CPP20(.value       =) tdl::IntValue{20, 1, 32}
                                },
                                tdl::Node{CPP20(.name        =) "window",
                                            CPP20(.description =) "The window size",
                                            CPP20(.tags        =) {"required"},
                                            CPP20(.value       =) tdl::IntValue{20}
                               },
                               tdl::Node{CPP20(.name        =) "single_input_file",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"input", "required", "file"},
                                            CPP20(.value       =) tdl::StringValue{}
                               },
                               tdl::Node{CPP20(.name        =) "optional_single_input_file",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"input", "file"},
                                            CPP20(.value       =) tdl::StringValue{}
                               },
                               tdl::Node{CPP20(.name        =) "multi_input_file",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"input", "required", "file"},
                                            CPP20(.value       =) tdl::StringValueList{}
                               },
                               tdl::Node{CPP20(.name        =) "single_input_directory",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"input", "required", "directory"},
                                            CPP20(.value       =) tdl::StringValue{}
                               },
                               tdl::Node{CPP20(.name        =) "single_output_file",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"output", "required", "file"},
                                            CPP20(.value       =) tdl::StringValue{}
                               },
                               tdl::Node{CPP20(.name        =) "optional_single_output_file",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"output", "file"},
                                            CPP20(.value       =) tdl::StringValue{}
                               },
                               tdl::Node{CPP20(.name        =) "prefixed_output_file",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"output", "prefixed", "required"},
                                            CPP20(.value       =) tdl::StringValue{}
                               },
                               tdl::Node{CPP20(.name        =) "optional_prefixed_output_file",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"output", "prefixed"},
                                            CPP20(.value       =) tdl::StringValue{}
                               },
                               tdl::Node{CPP20(.name        =) "prefixed_output_files",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"output", "prefixed", "required"},
                                            CPP20(.value       =) tdl::StringValueList{}
                               },
                               tdl::Node{CPP20(.name        =) "optional_prefixed_output_files",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"output", "prefixed"},
                                            CPP20(.value       =) tdl::StringValueList{}
                               },
                               tdl::Node{CPP20(.name        =) "single_output_dir",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"output", "required", "directory"},
                                            CPP20(.value       =) tdl::StringValue{}
                               },
                               tdl::Node{CPP20(.name        =) "optional_param1",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {},
                                            CPP20(.value       =) tdl::StringValue{}
                               },
                               tdl::Node{CPP20(.name        =) "optional_multi_input_file",
                                            CPP20(.description =) "no doc",
                                            CPP20(.tags        =) {"input", "file"},
                                            CPP20(.value       =) tdl::StringValueList{}
                               },
                               tdl::Node{CPP20(.name        =) "no_cli_binding",
                                            CPP20(.description =) "A parameter that has no corresponding cli binding."
                                                                  " Can only be filled via an input.json file.",
                                            CPP20(.tags        =) {},
                                            CPP20(.value       =) tdl::StringValue{}
                               },
                        }
                    }
            },
            CPP20(.cliMapping =) {
                {CPP20(.optionIdentifier =) "--kmer",                           CPP20(.referenceName =) "kmer"},
                {CPP20(.optionIdentifier =) "--window",                         CPP20(.referenceName =) "window"},
                {CPP20(.optionIdentifier =) "--single_input_file",              CPP20(.referenceName =) "single_input_file"},
                {CPP20(.optionIdentifier =) "--optional_single_input_file",     CPP20(.referenceName =) "optional_single_input_file"},
                {CPP20(.optionIdentifier =) "--multi_input_file",               CPP20(.referenceName =) "multi_input_file"},
                {CPP20(.optionIdentifier =) "--single_input_directory",         CPP20(.referenceName =) "single_input_directory"},
                {CPP20(.optionIdentifier =) "--single_output_file",             CPP20(.referenceName =) "single_output_file"},
                {CPP20(.optionIdentifier =) "--optional_single_output_file",    CPP20(.referenceName =) "optional_single_output_file"},
                {CPP20(.optionIdentifier =) "--prefixed_output_file",           CPP20(.referenceName =) "prefixed_output_file"},
                {CPP20(.optionIdentifier =) "--optional_prefixed_output_file",  CPP20(.referenceName =) "optional_prefixed_output_file"},
                {CPP20(.optionIdentifier =) "--prefixed_output_files",          CPP20(.referenceName =) "prefixed_output_files"},
                {CPP20(.optionIdentifier =) "--optional_prefixed_output_files", CPP20(.referenceName =) "optional_prefixed_output_files"},
                {CPP20(.optionIdentifier =) "--single_output_dir",              CPP20(.referenceName =) "single_output_dir"},
                {CPP20(.optionIdentifier =) "--optional_param1",                CPP20(.referenceName =) "optional_param1"},
                {CPP20(.optionIdentifier =) "--optional_multi_input_file",      CPP20(.referenceName =) "optional_multi_input_file"},
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
    type: long
    inputBinding:
      prefix: --window
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
    type: string?
    inputBinding:
      prefix: --optional_param1
  optional_multi_input_file:
    doc: no doc
    type: File[]?
    inputBinding:
      prefix: --optional_multi_input_file
  no_cli_binding:
    doc: A parameter that has no corresponding cli binding. Can only be filled via an input.json file.
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

}
void testCWL() {
    cwl_tests::testToolInfo();
    cwl_tests::testComplexCall();
}
