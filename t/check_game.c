// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "../src/chess/chess.h"

#include <check.h>

static const uint64_t START = 0xffff00000000ffff;

static inline uint64_t lift(uint64_t boardstate, enum Square square) {
    return boardstate & ~(1ul << square);
}

static inline uint64_t place(uint64_t boardstate, enum Square square) {
    return boardstate | (1ul << square);
}

static inline uint64_t
move(uint64_t boardstate, enum Square from, enum Square to) {
    return place(lift(boardstate, from), to);
}

// Initialized to starting position
START_TEST(test_starting_position)
{
    struct Game g; /* = */ game_from_fen(&g, NULL);
    struct Position *p = g.history.prev;
    ck_assert_int_eq(p->bitmap, START);

    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    game_destroy(&g);
}
END_TEST

// White has 20 choices to start
START_TEST(test_opening_moves)
{
    struct Game g; /* = */ game_from_fen(&g, NULL);
    ck_assert_int_eq(movelist_length(&g.moves), 20);
    game_destroy(&g);
}
END_TEST

// Play "1. e4"
START_TEST(test_open_e4)
{
    struct Game g; /* = */ game_from_fen(&g, NULL);
    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    game_move(&g, &e4);
    struct Position *p = g.history.prev;

    // Pawn has moved
    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

    // Bitmap reflects pawn move
    ck_assert_int_eq(p->bitmap, move(START, E2, E4));

    game_destroy(&g);
}
END_TEST

// Play "1. e4 e5"
START_TEST(test_open_e4e5)
{
    struct Game g; /* = */ game_from_fen(&g, NULL);

    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    struct Move e5 = { .from = E7, .to = E5, .promotion = EMPTY };
    game_move(&g, &e4);
    game_move(&g, &e5);

    uint64_t boardstate = move(START, E2, E4);
    boardstate = move(boardstate, E7, E5);
    struct Position *p = g.history.prev;
    ck_assert_int_eq(p->bitmap, boardstate);

    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2");

    game_destroy(&g);
}
END_TEST

// Read "1. e4" from a bitmap
START_TEST(test_read_e4)
{
    struct Game g; /* = */ game_from_fen(&g, NULL);
    const uint64_t boardstate = move(START, E2, E4);

    struct Move *move = NULL, *takeback = NULL;
    bool incomplete = false, promotion = false;
    struct Position *p = game_read_move(
        &g, boardstate, &move, &takeback, &incomplete, &promotion);

    // Got move and new position
    ck_assert_ptr_nonnull(p);
    ck_assert_ptr_nonnull(move);
    ck_assert_ptr_null(takeback);
    ck_assert(!incomplete);
    ck_assert(!promotion);

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
    struct Game g; /* = */ game_from_fen(&g, NULL);
    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    game_move(&g, &e4);

    uint64_t boardstate = move(START, E2, E4);
    boardstate = move(boardstate, E7, E5);

    struct Move *move = NULL, *takeback = NULL;
    bool incomplete = false, promotion = false;
    struct Position *p = game_read_move(
        &g, boardstate, &move, &takeback, &incomplete, &promotion);

    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2");

    game_destroy(&g);
}
END_TEST

// Takeback "1. ... e5"
START_TEST(test_read_takeback)
{
    struct Game g; /* = */ game_from_fen(&g, NULL);

    // Play "1. e4 e5"
    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    struct Move e5 = { .from = E7, .to = E5, .promotion = EMPTY };
    game_move(&g, &e4);
    game_move(&g, &e5);

    // N.B., name conflict
    const uint64_t boardstate = move(START, E2, E4);

    struct Move *move = NULL, *takeback = NULL;
    bool incomplete = false, promotion = false;
    struct Position *p = game_read_move(
        &g, boardstate, &move, &takeback, &incomplete, &promotion);

    // Got move and new position
    ck_assert_ptr_nonnull(p);
    ck_assert_ptr_nonnull(takeback);
    ck_assert_ptr_null(move);
    ck_assert(!incomplete);
    ck_assert(!promotion);

    // Position matches "1. e4"
    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

    game_destroy(&g);
}
END_TEST

static struct Position*
read_forward(struct Game *g, uint64_t boardstate, struct Move **move, bool *incomplete) {
    struct Move *takeback = NULL;
    bool promotion = false;
    return game_read_move(g, boardstate, move, &takeback, incomplete, &promotion);
}

START_TEST(test_illegal)
{
    struct Game g; /* = */ game_from_fen(&g, NULL);

    struct Move *move = NULL;
    bool incomplete = false;
    struct Position *p = read_forward(&g, lift(START, F1), &move, &incomplete);
    ck_assert_ptr_null(p);
    ck_assert(!incomplete);

    game_destroy(&g);
}
END_TEST

START_TEST(test_incomplete)
{
    struct Game g; /* = */ game_from_fen(&g, NULL);

    struct Move *move = NULL;
    bool incomplete = false;
    struct Position *p = read_forward(&g, lift(START, G1), &move, &incomplete);
    ck_assert_ptr_null(p);
    ck_assert(incomplete);

    game_destroy(&g);
}
END_TEST

START_TEST(test_out_of_turn)
{
    struct Game g; /* = */ game_from_fen(&g, NULL);

    struct Move *move = NULL;
    bool incomplete = false;
    struct Position *p = read_forward(&g, lift(START, E7), &move, &incomplete);
    ck_assert_ptr_null(p);
    ck_assert(!incomplete);

    game_destroy(&g);
}
END_TEST

// TODO test promotion

Suite *game_suite(void) {
    TCase *tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_starting_position);
    tcase_add_test(tc_core, test_opening_moves);
    tcase_add_test(tc_core, test_open_e4);
    tcase_add_test(tc_core, test_open_e4e5);
    tcase_add_test(tc_core, test_read_e4);
    tcase_add_test(tc_core, test_read_e4e5);
    tcase_add_test(tc_core, test_read_takeback);
    tcase_add_test(tc_core, test_illegal);
    tcase_add_test(tc_core, test_incomplete);
    tcase_add_test(tc_core, test_out_of_turn);

    Suite *s = suite_create("game");
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
