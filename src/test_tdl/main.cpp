// SPDX-FileCopyrightText: 2006-2024, Knut Reinert & Freie Universität Berlin
// SPDX-FileCopyrightText: 2016-2024, Knut Reinert & MPI für molekulare Genetik
// SPDX-License-Identifier: CC0-1.0

#include "utils.h"

void testCTD();
void testCWL();

int main() {
    testCTD();
    testCWL();
    std::cout << "Success! Nothing failed!\n";
    return EXIT_SUCCESS;
}
