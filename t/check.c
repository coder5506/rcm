// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include <stdlib.h>
#include <check.h>

Suite *game_suite(void);
Suite *pgn_suite(void);
Suite *san_suite(void);

static Suite *chess_suite(void) {
    return suite_create("chess");
}

int main(void) {
    SRunner *sr = srunner_create(chess_suite());
    srunner_add_suite(sr, game_suite());
    srunner_add_suite(sr, pgn_suite());
    srunner_add_suite(sr, san_suite());

    srunner_run_all(sr, CK_NORMAL);
    const int failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// This file is part of the Raccoon's Centaur Mods (RCM).
//
// RCM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RCM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
