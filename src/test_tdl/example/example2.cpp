// SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: CC0-1.0

#include <tdl/tdl.h>

// clang-format off
int main(int, char** argv) {
auto toolInfo = tdl::ToolInfo {
  .metaInfo = {
    .version = "7.6.5",                // version of your tool
    .name    = "testApp",              // name of your tool
    .docurl  = "example.com",          // url to the documentation of your tool
    .category = "sequence analysis",   //!TODO what are good categories?
    .description = "some description", // a description of your tool
    .executableName = argv[0],         // executable name of your tool, we advise to fill it with what ever is given in argv[0]

     // A list of citations you would like people to look at when they use your tool for research
    .citations      = {{"doi:123", "https://en.wikipedia.org/wiki/Meaning_of_life"},
                       {"doi:456", "https://en.wikipedia.org/wiki/Turing_completeness"}}
  }
};

// allow a call like `./myTool build --input <file1> <file2>`
auto node = tdl::Node {
  .name        = "build",
  .description = "builds some index for search",
  .tags        = {}, // no tags
  .value       = tdl::Node::Children { tdl::Node {
      .name        = "input", // This must be the same as the referenceName of CLIMapping
      .description = "input file",
      .value       = tdl::StringValueList{}, // indicates that we are accepting a list of strings as values
  }}
};
toolInfo.params.push_back(node);

toolInfo.cliMapping = {
       tdl::CLIMapping {.optionIdentifier = "--input",   .referenceName = "input"},
       tdl::CLIMapping {.optionIdentifier = "--index",   .referenceName = "index"},
       tdl::CLIMapping {.optionIdentifier = "--queries", .referenceName = "queries"}
  };

(void) toolInfo;
}
// clang-format on
