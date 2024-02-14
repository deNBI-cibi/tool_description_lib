# SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
# SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required (VERSION 3.12)

include (${CMAKE_CURRENT_LIST_DIR}/version.cmake)
include (GNUInstallDirs)

# If yaml-cpp is not installed, we build it as part of the tdl target. In this case, tdl is linked against a target
# that is also built by this project. If we want to install tdl, CMake wants us to also install yaml-cpp, because
# tdl depends on it. That is, if yaml-cpp is not installed, we have to install it.
if (TARGET yaml-cpp)
    set (TDL_EXPORT_YAML_CPP "yaml-cpp")
endif ()

install (TARGETS tdl ${TDL_EXPORT_YAML_CPP}
         EXPORT tdl_targets
         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
         FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})

install (DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../src/tdl" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install (EXPORT tdl_targets
         NAMESPACE tdl::
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tdl
         EXPORT_LINK_INTERFACE_LIBRARIES
         FILE tdl-targets.cmake)

# Since we do a `find_package (yaml-cpp)`, we need to do a `find_dependency (yaml-cpp)` in the package config file.
include (CMakePackageConfigHelpers)
configure_package_config_file (cmake/tdl-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/tdl-config.cmake
                               INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tdl)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/tdl-config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tdl)

set (version_file "${CMAKE_CURRENT_BINARY_DIR}/cmake/tdl-config-version.cmake")
write_basic_package_version_file (
    ${version_file}
    VERSION "${TDL_VERSION}"
    COMPATIBILITY AnyNewerVersion)
install (FILES ${version_file} DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tdl)

install (FILES "${CMAKE_CURRENT_LIST_DIR}/../README.md" TYPE DOC)
install (DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../LICENSES" TYPE DOC)
