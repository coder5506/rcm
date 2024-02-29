// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#include "../src/chess/chess.h"

#include <stdio.h>
#include <stdlib.h>

#include <check.h>

// Initialized to starting position
START_TEST(test_starting_position)
{
    struct Game g; /* = */ game_init(&g, NULL);
    struct Position *p = g.history.prev;
    ck_assert_int_eq(p->bitmap, 0xffff00000000ffff);

    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    game_destroy(&g);
}
END_TEST

// White has 20 choices to start
START_TEST(test_opening_moves)
{
    struct Game g; /* = */ game_init(&g, NULL);
    ck_assert_int_eq(movelist_length(&g.moves), 20);
    game_destroy(&g);
}
END_TEST

// Play "1. e4"
START_TEST(test_open_e4)
{
    struct Game g; /* = */ game_init(&g, NULL);
    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    game_move(&g, &e4);
    struct Position *p = g.history.prev;

    // Pawn has moved
    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

    // Bitmap reflects pawn move
    ck_assert_int_eq(p->bitmap, 0xffef00100000ffff);

    game_destroy(&g);
}
END_TEST

// Play "1. e4 e5"
START_TEST(test_open_e4e5)
{
    struct Game g; /* = */ game_init(&g, NULL);

    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    struct Move e5 = { .from = E7, .to = E5, .promotion = EMPTY };
    game_move(&g, &e4);
    game_move(&g, &e5);

    struct Position *p = g.history.prev;
    ck_assert_int_eq(p->bitmap, 0xffef00101000efff);

    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2");

    game_destroy(&g);
}
END_TEST

// Read "1. e4" from a bitmap
START_TEST(test_read_e4)
{
    struct Game g; /* = */ game_init(&g, NULL);

    struct Move *move     = NULL;
    struct Move *takeback = NULL;
    bool incomplete = false;
    bool promotion  = false;
    struct Position *p = game_read_move(
        &g, 0xffef00100000ffff, &move, &takeback, &incomplete, &promotion);

    // Got move and new position
    ck_assert_ptr_nonnull(p);
    ck_assert_ptr_nonnull(move);
    ck_assert_ptr_null(takeback);

    // Position matches "1. e4"
    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

    game_destroy(&g);
}
END_TEST

// Read "1. ... e5" from a bitmap
START_TEST(test_read_e4e5)
{
    struct Game g; /* = */ game_init(&g, NULL);
    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    game_move(&g, &e4);

    struct Move *move     = NULL;
    struct Move *takeback = NULL;
    bool incomplete = false;
    bool promotion  = false;
    struct Position *p = game_read_move(
        &g, 0xffef00101000efff, &move, &takeback, &incomplete, &promotion);

    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2");

    game_destroy(&g);
}
END_TEST

// Takeback "1. ... e5"
START_TEST(test_read_takeback)
{
    struct Game g; /* = */ game_init(&g, NULL);

    // Play "1. e4 e5"
    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    struct Move e5 = { .from = E7, .to = E5, .promotion = EMPTY };
    game_move(&g, &e4);
    game_move(&g, &e5);

    struct Move *move     = NULL;
    struct Move *takeback = NULL;
    bool incomplete = false;
    bool promotion  = false;
    struct Position *p = game_read_move(
        &g, 0xffef00100000ffff, &move, &takeback, &incomplete, &promotion);

    // Got move and new position
    ck_assert_ptr_nonnull(p);
    ck_assert_ptr_nonnull(takeback);
    ck_assert_ptr_null(move);

    // Position matches "1. e4"
    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

    game_destroy(&g);
}
END_TEST

static Suite *position_suite(void) {
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_starting_position);
    tcase_add_test(tc_core, test_opening_moves);
    tcase_add_test(tc_core, test_open_e4);
    tcase_add_test(tc_core, test_open_e4e5);
    tcase_add_test(tc_core, test_read_e4);
    tcase_add_test(tc_core, test_read_e4e5);
    tcase_add_test(tc_core, test_read_takeback);

    Suite *s = suite_create("Position");
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    Suite *s = position_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
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
