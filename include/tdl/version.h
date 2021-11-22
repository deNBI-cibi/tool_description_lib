#pragma once

#include <cstddef>
#include <cstdint>

/*!\file
 * \brief Provides TDL version macros and global variables.
 * \author Svenja Mehringer <svenja.mehringer AT fu-berlin.de>
 */

//!\brief The major version as MACRO.
#define TDL_VERSION_MAJOR     0
//!\brief The minor version as MACRO.
#define TDL_VERSION_MINOR     1
//!\brief The patch version as MACRO.
#define TDL_VERSION_PATCH     0
//!\brief The release candidate number. 0 means stable release, >= 1 means release candidate.
#define TDL_RELEASE_CANDIDATE 1

//!\brief The full version as MACRO (number).
#define TDL_VERSION (TDL_VERSION_MAJOR * 10000 + TDL_VERSION_MINOR * 100 + TDL_VERSION_PATCH)

/*!\brief Converts a number to a string. Preprocessor needs this indirection to
 * properly expand the values to strings.
 */
#define TDL_VERSION_CSTRING_HELPER_STR(str) #str

//!\brief Converts version numbers to string.
#define TDL_VERSION_CSTRING_HELPER_FUNC(MAJOR, MINOR, PATCH)                                                           \
    TDL_VERSION_CSTRING_HELPER_STR(MAJOR)                                                                              \
    "." TDL_VERSION_CSTRING_HELPER_STR(MINOR) "." TDL_VERSION_CSTRING_HELPER_STR(PATCH)

#if (TDL_RELEASE_CANDIDATE > 0)
//!\brief A helper function that expands to a suitable release candidate suffix.
#    define TDL_RELEASE_CANDIDATE_HELPER(RC) "-rc." TDL_VERSION_CSTRING_HELPER_STR(RC)
#else
//!\brief A helper function that expands to a suitable release candidate suffix.
#    define TDL_RELEASE_CANDIDATE_HELPER(RC) ""
#endif

//!\brief The full version as null terminated string.
#define TDL_VERSION_CSTRING                                                                                            \
    TDL_VERSION_CSTRING_HELPER_FUNC(TDL_VERSION_MAJOR, TDL_VERSION_MINOR, TDL_VERSION_PATCH)                           \
    TDL_RELEASE_CANDIDATE_HELPER(TDL_RELEASE_CANDIDATE)

namespace tdl
{

//!\brief The major version.
constexpr uint8_t tdl_version_major = TDL_VERSION_MAJOR;
//!\brief The minor version.
constexpr uint8_t tdl_version_minor = TDL_VERSION_MINOR;
//!\brief The patch version.
constexpr uint8_t tdl_version_patch = TDL_VERSION_PATCH;

//!\brief The full version as `std::size_t`.
constexpr std::size_t tdl_version = TDL_VERSION;

//!\brief The full version as null terminated string.
constexpr char const * tdl_version_cstring = TDL_VERSION_CSTRING;

} // namespace tdl

#undef TDL_VERSION_CSTRING_HELPER_STR
#undef TDL_VERSION_CSTRING_HELPER_FUNC
#undef TDL_RELEASE_CANDIDATE_HELPER
