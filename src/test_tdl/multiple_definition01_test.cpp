// SPDX-FileCopyrightText: 2006-2023, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2023, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: CC0-1.0

#include <tdl/ToolInfo.h>
#include <tdl/convertToCTD.h>


/* This file, in combination with multiple_definition02_test.cpp are testing for multiple definition errors.
 * Since this is a header only library, many functions are marked with `inline`. To avoid missing an `inline`
 * keyword, we have two translation units with the same symbols. Linking these together will cause an multiple
 * definition violation when the keyword `inline` is missing.
 */
