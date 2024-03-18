// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "../src/chess/chess.h"

#include <check.h>

START_TEST(test_white_legal) {
    struct Position *p = position_from_fen(NULL);

    struct Move *m = move_from_san(p, "e4");
    ck_assert_ptr_nonnull(m);
    ck_assert_int_eq(m->from, E2);
    ck_assert_int_eq(m->to, E4);
    ck_assert_int_eq(m->promotion, ' ');
    ck_assert_str_eq(move_san_static(m), "e4");

    m = move_from_san(p, "Nf3");
    ck_assert_ptr_nonnull(m);
    ck_assert_int_eq(m->from, G1);
    ck_assert_int_eq(m->to, F3);
    ck_assert_int_eq(m->promotion, ' ');
    ck_assert_str_eq(move_san_static(m), "Nf3");
}
END_TEST

START_TEST(test_white_illegal) {
    struct Position *p = position_from_fen(NULL);

    struct Move *m = move_from_san(p, "e5");
    ck_assert_ptr_null(m);

    m = move_from_san(p, "Bd3");
    ck_assert_ptr_null(m);
}
END_TEST

START_TEST(test_black_legal) {
    struct Position *p = position_from_fen(NULL);
    struct Move *m = move_from_san(p, "e4");
    p = position_apply_move(p, m);

    m = move_from_san(p, "e5");
    ck_assert_ptr_nonnull(m);
    ck_assert_int_eq(m->from, E7);
    ck_assert_int_eq(m->to, E5);
    ck_assert_int_eq(m->promotion, ' ');
    ck_assert_str_eq(move_san_static(m), "e5");

    m = move_from_san(p, "Nf6");
    ck_assert_ptr_nonnull(m);
    ck_assert_int_eq(m->from, G8);
    ck_assert_int_eq(m->to, F6);
    ck_assert_int_eq(m->promotion, ' ');
    ck_assert_str_eq(move_san_static(m), "Nf6");
}
END_TEST

START_TEST(test_black_illegal) {
    struct Position *p = position_from_fen(NULL);
    struct Move *m = move_from_san(p, "e4");
    p = position_apply_move(p, m);

    m = move_from_san(p, "e4");
    ck_assert_ptr_null(m);

    m = move_from_san(p, "Bd6");
    ck_assert_ptr_null(m);
}
END_TEST

Suite *san_suite(void) {
    TCase *tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_white_legal);
    tcase_add_test(tc_core, test_white_illegal);
    tcase_add_test(tc_core, test_black_legal);
    tcase_add_test(tc_core, test_black_illegal);

    Suite *s = suite_create("san");
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
