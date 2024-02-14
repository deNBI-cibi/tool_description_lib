// SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <functional>

#include <yaml-cpp/node/node.h>

#include "ToolInfo.h"

namespace tdl {

/**!\brief a global callback function to adjust the exporting for cwl
 *
 * This callback allows to adjust the exported yaml file to add/change/remove
 * cwl entries, which currently aren't controllable via tdl itself.
 */
inline std::function<void(YAML::Node &)> post_process_cwl;

/*!\brief converts a ToolInfo into a string that
 * holds the CWL representation of the given tool
 */
auto convertToCWL(ToolInfo const & doc) -> std::string;

} // namespace tdl
