// SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: CC0-1.0

#pragma once

#include <cassert>
#include <iostream>

#include <tdl/tdl.h>

#if __cpp_designated_initializers >= 201707L
#    define DESINIT(x) x
#else
#    define DESINIT(x)
#endif
