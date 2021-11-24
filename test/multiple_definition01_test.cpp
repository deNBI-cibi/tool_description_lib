#include <gtest/gtest.h>

#include <tdl/Param.h>
#include <tdl/ParamCTDFile.h>
#include <tdl/ParamValue.h>
#include <tdl/platform.h>
#include <tdl/version.h>




/* This file, in combination with multiple_definition02_test.cpp are testing for multiple definition errors.
 * Since this is a header only library, many functions are marked with `inline`. To avoid missing an `inline`
 * keyword, we have two translation units with the same symbols. Linking these together will cause an multiple
 * definition violation when the keyword `inline` is missing.
 */
