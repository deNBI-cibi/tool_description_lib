// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2023, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2023, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file.
// -----------------------------------------------------------------------------------------------------

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
                tdl::Node{CPP20(.name        =) "command",
                          CPP20(.description =) "Command",
                          CPP20(.tags        =) {},
                          CPP20(.value       =) tdl::StringValue{"build"},
                },
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
            },
            CPP20(.cliMapping =) {
                {CPP20(.optionIdentifier =) "",         CPP20(.referenceName    =) "command"},
                {CPP20(.optionIdentifier =) "--kmer",   CPP20(.referenceName    =) "kmer"},
                {CPP20(.optionIdentifier =) "--window", CPP20(.referenceName    =) "window"},
            },
        });
        auto expected = std::string{R"(inputs:
  - doc: The k-mer size
    id: kmer
    type: long
    inputBinding:
      prefix: --kmer
  - doc: The window size
    id: window
    type: long
    inputBinding:
      prefix: --window
outputs:
  []
label: ""
doc: ""
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
                tdl::Node{CPP20(.name        =) "command",
                          CPP20(.description =) "Command",
                          CPP20(.tags        =) {},
                          CPP20(.value       =) tdl::StringValue{"build"},
                },
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
               tdl::Node{CPP20(.name        =) "prefixed_output_file",
                            CPP20(.description =) "no doc",
                            CPP20(.tags        =) {"output", "prefixed", "required"},
                            CPP20(.value       =) tdl::StringValue{}
               },
               tdl::Node{CPP20(.name        =) "prefixed_output_files",
                            CPP20(.description =) "no doc",
                            CPP20(.tags        =) {"output", "prefixed", "required"},
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
            },
            CPP20(.cliMapping =) {
                {CPP20(.optionIdentifier =) "",                            CPP20(.referenceName =) "command"},
                {CPP20(.optionIdentifier =) "--kmer",                      CPP20(.referenceName =) "kmer"},
                {CPP20(.optionIdentifier =) "--window",                    CPP20(.referenceName =) "window"},
                {CPP20(.optionIdentifier =) "--single_input_file",         CPP20(.referenceName =) "single_input_file"},
                {CPP20(.optionIdentifier =) "--multi_input_file",          CPP20(.referenceName =) "multi_input_file"},
                {CPP20(.optionIdentifier =) "--single_input_directory",    CPP20(.referenceName =) "single_input_directory"},
                {CPP20(.optionIdentifier =) "--single_output_file",        CPP20(.referenceName =) "single_output_file"},
                {CPP20(.optionIdentifier =) "--prefixed_output_file" ,     CPP20(.referenceName =) "prefixed_output_file"},
                {CPP20(.optionIdentifier =) "--prefixed_output_files",     CPP20(.referenceName =) "prefixed_output_files"},
                {CPP20(.optionIdentifier =) "--single_output_dir",         CPP20(.referenceName =) "single_output_dir"},
                {CPP20(.optionIdentifier =) "--optional_param1",           CPP20(.referenceName =) "optional_param1"},
                {CPP20(.optionIdentifier =) "--optional_multi_input_file", CPP20(.referenceName =) "optional_multi_input_file"},
            },
        });
        auto expected = std::string{R"(inputs:
  - doc: The k-mer size
    id: kmer
    type: long
    inputBinding:
      prefix: --kmer
  - doc: The window size
    id: window
    type: long
    inputBinding:
      prefix: --window
  - doc: no doc
    id: single_input_file
    type: File
    inputBinding:
      prefix: --single_input_file
  - doc: no doc
    id: multi_input_file
    type: File[]
    inputBinding:
      prefix: --multi_input_file
  - doc: no doc
    id: single_input_directory
    type: Directory
    inputBinding:
      prefix: --single_input_directory
  - doc: no doc
    id: single_output_file
    type: string
    inputBinding:
      prefix: --single_output_file
  - doc: no doc
    id: prefixed_output_file
    type: string
    inputBinding:
      prefix: --prefixed_output_file
  - doc: no doc
    id: prefixed_output_files
    type: string
    inputBinding:
      prefix: --prefixed_output_files
  - doc: no doc
    id: single_output_dir
    type: string
    inputBinding:
      prefix: --single_output_dir
  - doc: no doc
    id: optional_param1
    type: string?
    inputBinding:
      prefix: --optional_param1
  - doc: no doc
    id: optional_multi_input_file
    type: File[]?
    inputBinding:
      prefix: --optional_multi_input_file
outputs:
  - id: single_output_file
    type: File
    outputBinding:
      glob: $(inputs.single_output_file)
  - id: prefixed_output_file
    type: File
    outputBinding:
      glob: $(inputs.prefixed_output_file)*
  - id: prefixed_output_files
    type: File[]
    outputBinding:
      glob: $(inputs.prefixed_output_files)*
  - id: single_output_dir
    type: Directory
    outputBinding:
      glob: $(inputs.single_output_dir)
label: ""
doc: ""
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
