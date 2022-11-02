// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file.
// -----------------------------------------------------------------------------------------------------

#include <tdl/tdl.h>

#include <cassert>
#include <iostream>

#if __cplusplus >= 202002L
#  define CPP20(x) x
#else
#  define CPP20(x)
#endif


namespace cwl_tests {

void testToolInfo() {
    { // a multi nested parameter
        auto output = convertToCWL(tdl::ToolInfo {
            CPP20(.metaInfo =) {},
            CPP20(.params =) {
                tdl::Node{CPP20(.name        =) "command",
                          CPP20(.description =) "Command",
                          CPP20(.tags        =) {},
                          CPP20(.value       =) tdl::StringValue{"build"},
                },
                tdl::Node{CPP20(.name        =) "kmer",
                            CPP20(.description =) "The k-mer size",
                            CPP20(.tags        =) {},
                            CPP20(.value       =) tdl::IntValue{20, 1, 32}
                },
                tdl::Node{CPP20(.name        =) "window",
                            CPP20(.description =) "The window size",
                            CPP20(.tags        =) {},
                            CPP20(.value       =) tdl::IntValue{20}
               }
            },
            CPP20(.cliMapping =) {
                {CPP20(.optionIdentifier =) "",         CPP20(.referenceName    =) "command"},
                {CPP20(.optionIdentifier =) "--kmer",   CPP20(.referenceName    =) "kmer"},
                {CPP20(.optionIdentifier =) "--window", CPP20(.referenceName    =) "window"},
            }
        });
        auto expected = std::string{R"(inputs:
  - id: kmer
    type: long
    inputBinding:
      prefix: --kmer
  - id: window
    type: long
    inputBinding:
      prefix: --window
outputs:
  []
id: ""
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
}
