// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "../src/chess/chess.h"
#include "../src/list.h"

#include <check.h>

static const uint64_t START = 0xffff00000000ffff;

static inline uint64_t lift(uint64_t boardstate, enum Square square) {
    return boardstate & ~(1ull << square);
}

static inline uint64_t place(uint64_t boardstate, enum Square square) {
    return boardstate | (1ull << square);
}

static inline uint64_t
move(uint64_t boardstate, enum Square from, enum Square to) {
    return place(lift(boardstate, from), to);
}

// Initialized to starting position
START_TEST(test_starting_position)
{
    struct Game *g = game_from_fen(NULL);
    struct Position *p = game_current(g);
    ck_assert_int_eq(p->bitmap, START);

    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}
END_TEST

// White has 20 choices to start
START_TEST(test_opening_moves)
{
    struct Game *g = game_from_fen(NULL);
    struct List *moves = game_legal_moves(g);
    ck_assert_int_eq(list_length(moves), 20);
}
END_TEST

// Play "1. e4"
START_TEST(test_open_e4)
{
    struct Game *g = game_from_fen(NULL);
    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    game_apply_move(g, &e4);
    struct Position *p = game_current(g);

    // Pawn has moved
    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

    // Bitmap reflects pawn move
    ck_assert_int_eq(p->bitmap, move(START, E2, E4));
}
END_TEST

// Play "1. e4 e5"
START_TEST(test_open_e4e5)
{
    struct Game *g = game_from_fen(NULL);

    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    struct Move e5 = { .from = E7, .to = E5, .promotion = EMPTY };
    game_apply_move(g, &e4);
    game_apply_move(g, &e5);

    uint64_t boardstate = move(START, E2, E4);
    boardstate = move(boardstate, E7, E5);
    struct Position *p = game_current(g);
    ck_assert_int_eq(p->bitmap, boardstate);

    char fen[FEN_MAX];
    position_fen(p, fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2");
}
END_TEST

// Read "1. e4" from a bitmap
START_TEST(test_read_e4)
{
    struct Game *g = game_from_fen(NULL);
    const uint64_t boardstate = move(START, E2, E4);

    struct List  *candidates = list_new();
    struct Move  *takeback   = NULL;
    struct Action actions[1];
    const bool maybe_valid =
        game_read_move(candidates, &takeback, g, boardstate, actions, 0);

    // Got move and new position
    ck_assert(maybe_valid);
    ck_assert_int_eq(list_length(candidates), 1);
    ck_assert_ptr_null(takeback);

    game_apply_move(g, list_pop(candidates));

    // Position matches "1. e4"
    char fen[FEN_MAX];
    position_fen(game_current(g), fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
}
END_TEST

// Read "1. ... e5" from a bitmap
START_TEST(test_read_e4e5)
{
    struct Game *g = game_from_fen(NULL);
    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    game_apply_move(g, &e4);

    uint64_t boardstate = move(START, E2, E4);
    boardstate = move(boardstate, E7, E5);

    struct List  *candidates = list_new();
    struct Move  *takeback   = NULL;
    struct Action actions[1];
    const bool maybe_valid =
        game_read_move(candidates, &takeback, g, boardstate, actions, 0);

    ck_assert(maybe_valid);
    game_apply_move(g, list_pop(candidates));

    char fen[FEN_MAX];
    position_fen(game_current(g), fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2");
}
END_TEST

// Takeback "1. ... e5"
START_TEST(test_read_takeback)
{
    struct Game *g = game_from_fen(NULL);

    // Play "1. e4 e5"
    struct Move e4 = { .from = E2, .to = E4, .promotion = EMPTY };
    struct Move e5 = { .from = E7, .to = E5, .promotion = EMPTY };
    game_apply_move(g, &e4);
    game_apply_move(g, &e5);

    // N.B., name conflict
    const uint64_t boardstate = move(START, E2, E4);

    struct List  *candidates = list_new();
    struct Move  *takeback   = NULL;
    struct Action actions[1];
    const bool maybe_valid =
        game_read_move(candidates, &takeback, g, boardstate, actions, 0);

    // Got move and new position
    ck_assert(maybe_valid);
    ck_assert_int_eq(list_length(candidates), 0);
    ck_assert_ptr_nonnull(takeback);

    game_apply_takeback(g, takeback);

    // Position matches "1. e4"
    char fen[FEN_MAX];
    position_fen(game_current(g), fen, sizeof fen);
    ck_assert_str_eq(fen, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
}
END_TEST

START_TEST(test_illegal)
{
    struct Game *g = game_from_fen(NULL);

    struct List  *candidates = list_new();
    struct Move  *takeback   = NULL;
    struct Action actions[1];
    const bool maybe_valid =
        game_read_move(candidates, &takeback, g, lift(START, F1), actions, 0);

    ck_assert(!maybe_valid);
}
END_TEST

START_TEST(test_incomplete)
{
    struct Game *g = game_from_fen(NULL);

    struct List  *candidates = list_new();
    struct Move  *takeback   = NULL;
    struct Action actions[1];
    const bool maybe_valid =
        game_read_move(candidates, &takeback, g, lift(START, G1), actions, 0);

    ck_assert(maybe_valid);
    ck_assert_int_eq(list_length(candidates), 0);
}
END_TEST

START_TEST(test_out_of_turn)
{
    struct Game *g = game_from_fen(NULL);

    struct List  *candidates = list_new();
    struct Move  *takeback   = NULL;
    struct Action actions[1];
    const bool maybe_valid =
        game_read_move(candidates, &takeback, g, lift(START, E7), actions, 0);

    ck_assert(!maybe_valid);
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
