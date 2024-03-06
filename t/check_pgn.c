// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "../src/chess/chess.h"

#include <stdarg.h>
#include <check.h>

static void play_san_moves(struct Game *game, ...) {
    va_list args; /* = */ va_start(args, game);
    const char *san = va_arg(args, char*);
    while (san) {
        game_move_san(game, san);
        san = va_arg(args, char*);
    }
    va_end(args);
}

START_TEST(test_steinitz)
{
    struct Game *g = game_from_fen(NULL);
    play_san_moves(g, "e4", "e5", "Nf3", "Nc6", "Bb5", "d6", "d4", NULL);
    ck_assert_str_eq(game_pgn(g), "1. e4 e5 2. Nf3 Nc6 3. Bb5 d6 4. d4");
}
END_TEST

START_TEST(test_semi_steinitz)
{
    struct Game *g = game_from_fen(NULL);
    play_san_moves(g, "e4", "e5", "Nf3", "Nc6", "Bb5", "a6", "Ba4", "d6", NULL);
    ck_assert_str_eq(game_pgn(g), "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 d6");

    game_takeback(g); game_takeback(g); game_takeback(g);
    play_san_moves(g, "d6", "d4", NULL);
    ck_assert_str_eq(game_pgn(g), "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 (3... d6 4. d4) 4. Ba4 d6");
}
END_TEST

Suite *pgn_suite(void) {
    TCase *tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_steinitz);
    tcase_add_test(tc_core, test_semi_steinitz);

    Suite *s = suite_create("pgn");
    suite_add_tcase(s, tc_core);
    return s;
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
