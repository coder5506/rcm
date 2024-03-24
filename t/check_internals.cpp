/****************************************************************************
 * ChessRules.cpp Chess classes - Rules of chess
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "../src/thc/ChessRules.h"
#include "doctest.h"

using namespace std;
using namespace thc;

// Adapted from ChessRules::TestInternals
TEST_CASE("test castling internals: rook moves") {
    ChessRules p;

    p.play_uci_move("g1f3");  // Nf3
    p.play_uci_move("g8f6");  // Nf6
    CHECK(int(p.d.wking)  == 1);  CHECK(p.wking_allowed());
    CHECK(int(p.d.bking)  == 1);  CHECK(p.bking_allowed());
    CHECK(int(p.d.wqueen) == 1);  CHECK(p.wqueen_allowed());
    CHECK(int(p.d.bqueen) == 1);  CHECK(p.bqueen_allowed());

    p.play_uci_move("h1g1");  // Rg1
    p.play_uci_move("h8g8");  // Rg8
    p.play_uci_move("g1h1");  // Rh1 -- return of rook to h1 clears wking
    CHECK(int(p.d.wking)  == 0);  CHECK(!p.wking_allowed());
    CHECK(int(p.d.bking)  == 1);  CHECK(!p.bking_allowed());

    p.play_uci_move("g8h8");  // Rh8 -- return of rook to h8 clears bking
    CHECK(int(p.d.wking)  == 0);  CHECK(!p.wking_allowed());
    CHECK(int(p.d.bking)  == 0);  CHECK(!p.bking_allowed());

    p.play_uci_move("b1c3");  // Nc3
    p.play_uci_move("b8c6");  // Nc6
    p.play_uci_move("a1b1");  // Rb1
    p.play_uci_move("a8b8");  // Rb8
    p.play_uci_move("b1a1");  // Ra1 -- return of rook to a1 clears wqueen
    CHECK(int(p.d.wqueen) == 0);  CHECK(!p.wqueen_allowed());
    CHECK(int(p.d.bqueen) == 1);  CHECK(!p.bqueen_allowed());

    p.play_uci_move("b8a8");  // Ra8 -- return of rook to a8 clears bqueen
    CHECK(int(p.d.wqueen) == 0);  CHECK(!p.wqueen_allowed());
    CHECK(int(p.d.bqueen) == 0);  CHECK(!p.bqueen_allowed());
}

// Adapted from ChessRules::TestInternals
TEST_CASE("test castling internals: king moves") {
    ChessRules p;

    p.play_uci_move("e2e3");  // e3
    p.play_uci_move("e7e6");  // e6
    CHECK(int(p.d.wking)  == 1);  CHECK(p.wking_allowed());
    CHECK(int(p.d.bking)  == 1);  CHECK(p.bking_allowed());
    CHECK(int(p.d.wqueen) == 1);  CHECK(p.wqueen_allowed());
    CHECK(int(p.d.bqueen) == 1);  CHECK(p.bqueen_allowed());

    p.play_uci_move("e1e2");  // Ke2
    p.play_uci_move("e8e7");  // Ke7
    p.play_uci_move("e2e1");  // Ke1 -- return of king to e1 clears wking, wqueen
    CHECK(int(p.d.wking)  == 0);  CHECK(!p.wking_allowed());
    CHECK(int(p.d.bking)  == 1);  CHECK(!p.bking_allowed());
    CHECK(int(p.d.wqueen) == 0);  CHECK(!p.wqueen_allowed());
    CHECK(int(p.d.bqueen) == 1);  CHECK(!p.bqueen_allowed());

    p.play_uci_move("e7e8");  // Ke8
    CHECK(int(p.d.wking)  == 0);  CHECK(!p.wking_allowed());
    CHECK(int(p.d.bking)  == 0);  CHECK(!p.bking_allowed());
    CHECK(int(p.d.wqueen) == 0);  CHECK(!p.wqueen_allowed());
    CHECK(int(p.d.bqueen) == 0);  CHECK(!p.bqueen_allowed());
}

// Adapted from ChessRules::TestInternals
TEST_CASE("test internals") {
    ChessRules p;
    Move m;

    p.Forsyth("b3k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1");
    CHECK(p.white);
    CHECK(p.half_move_clock == 0);
    CHECK(p.full_move_count == 1);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == e1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 1);  CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bking)  == 1);  CHECK(int(p.d.bqueen) == 0);
    CHECK(p.wking_allowed());


    m.TerseIn(&p, "h1h2");  // 1. Rh2
    p.PushMove(m);
    CHECK(!p.white);
    CHECK(p.half_move_clock == 0);  // only updated by PlayMove
    CHECK(p.full_move_count == 1);
    CHECK(int(p.d.wking)  == 1);  CHECK(!p.wking_allowed());
    CHECK(p.at(h1) == ' ');  CHECK(p.at(h2) == 'R');

    p.PopMove(m);  // takeback 1. Rh2
    CHECK(p.white);
    CHECK(int(p.d.wking)  == 1);  CHECK(p.wking_allowed());
    CHECK(p.at(h1) == 'R');  CHECK(p.at(h2) == ' ');

    p.PlayMove(m);  // 1. Rh2
    CHECK(!p.white);
    CHECK(p.half_move_clock == 1);
    CHECK(p.full_move_count == 1);
    CHECK(int(p.d.wking)  == 1);  CHECK(!p.wking_allowed());
    CHECK(p.at(h1) == ' ');  CHECK(p.at(h2) == 'R');


    m.TerseIn(&p, "a8h1");  // 1... Bh1
    p.PushMove(m);
    CHECK(p.white);
    CHECK(p.half_move_clock == 1);
    CHECK(p.full_move_count == 1);  // only updated by PlayMove
    CHECK(int(p.d.wking)  == 0);
    CHECK(p.at(a8) == ' ');  CHECK(p.at(h1) == 'b');

    p.PopMove(m);  // takeback 1... Bh1
    CHECK(!p.white);
    CHECK(int(p.d.wking)  == 1);
    CHECK(p.at(a8) == 'b');  CHECK(p.at(h1) == ' ');

    p.PlayMove(m);  // 1... Bh1
    CHECK(p.white);
    CHECK(p.half_move_clock == 2);
    CHECK(p.full_move_count == 2);
    CHECK(int(p.d.wking)  == 0);
    CHECK(p.at(a8) == ' ');  CHECK(p.at(h1) == 'b');


    m.TerseIn(&p, "e1c1");  // 2. O-O-O
    p.PushMove(m);
    CHECK(!p.white);
    CHECK(p.half_move_clock == 2);
    CHECK(p.full_move_count == 2);
    CHECK(int(p.d.wking_square) == c1);
    CHECK(int(p.d.wking)  == 0);  CHECK(int(p.d.wqueen) == 1);
    CHECK(p.at(a1) == ' ');  CHECK(p.at(d1) == 'R');
    CHECK(p.at(c1) == 'K');  CHECK(p.at(e1) == ' ');

    p.PopMove(m);  // takeback 2. O-O-O
    CHECK(p.white);
    CHECK(int(p.d.wking_square) == e1);
    CHECK(int(p.d.wking)  == 0);  CHECK(int(p.d.wqueen) == 1);
    CHECK(p.at(a1) == 'R');  CHECK(p.at(d1) == ' ');
    CHECK(p.at(c1) == ' ');  CHECK(p.at(e1) == 'K');

    p.PlayMove(m);  // 2. O-O-O
    CHECK(!p.white);
    CHECK(p.half_move_clock == 3);
    CHECK(p.full_move_count == 2);
    CHECK(int(p.d.wking_square) == c1);
    CHECK(int(p.d.wking)  == 0);  CHECK(int(p.d.wqueen) == 1);
    CHECK(p.at(a1) == ' ');  CHECK(p.at(d1) == 'R');
    CHECK(p.at(c1) == 'K');  CHECK(p.at(e1) == ' ');


    m.TerseIn(&p, "h1a8");  // 2... Ba8
    p.PushMove(m);
    CHECK(p.white);
    CHECK(p.half_move_clock == 3);
    CHECK(p.full_move_count == 2);
    CHECK(p.at(a8) == 'b');  CHECK(p.at(h1) == ' ');

    p.PopMove(m);  // takeback 2... Ba8
    CHECK(!p.white);
    CHECK(p.at(a8) == ' ');  CHECK(p.at(h1) == 'b');

    p.PlayMove(m);  // 2... Ba8
    CHECK(p.white);
    CHECK(p.half_move_clock == 4);
    CHECK(p.full_move_count == 3);
    CHECK(p.at(b1) == ' ');  CHECK(p.at(c1) == 'K');


    m.TerseIn(&p, "c1b1");  // 3. Kb1
    p.PushMove(m);
    CHECK(!p.white);
    CHECK(p.half_move_clock == 4);
    CHECK(p.full_move_count == 3);
    CHECK(int(p.d.wking_square) == b1);
    CHECK(int(p.d.wking)  == 0);  CHECK(int(p.d.wqueen) == 1);
    CHECK(p.at(b1) == 'K');  CHECK(p.at(c1) == ' ');

    p.PopMove(m);  // takeback 3. Kb1
    CHECK(p.white);
    CHECK(int(p.d.wking_square) == c1);
    CHECK(int(p.d.wking)  == 0);  CHECK(int(p.d.wqueen) == 1);
    CHECK(p.at(b1) == ' ');  CHECK(p.at(c1) == 'K');

    p.PlayMove(m);  // 3. Kb1
    CHECK(!p.white);
    CHECK(p.half_move_clock == 5);
    CHECK(p.full_move_count == 3);
    CHECK(int(p.d.wking_square) == b1);
    CHECK(int(p.d.wking)  == 0);  CHECK(int(p.d.wqueen) == 1);
    CHECK(p.at(b1) == 'K');  CHECK(p.at(c1) == ' ');


    m.TerseIn(&p, "e8g8");  // 3... O-O
    p.PushMove(m);
    CHECK(p.white);
    CHECK(p.half_move_clock == 5);
    CHECK(p.full_move_count == 3);
    CHECK(int(p.d.bking_square) == g8);
    CHECK(int(p.d.bking)  == 1);  CHECK(int(p.d.bqueen) == 0);
    CHECK(p.at(e8) == ' ');  CHECK(p.at(g8) == 'k');
    CHECK(p.at(f8) == 'r');  CHECK(p.at(h8) == ' ');

    p.PopMove(m);  // takeback 3... O-O
    CHECK(!p.white);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.bking)  == 1);  CHECK(int(p.d.bqueen) == 0);
    CHECK(p.at(e8) == 'k');  CHECK(p.at(g8) == ' ');
    CHECK(p.at(f8) == ' ');  CHECK(p.at(h8) == 'r');

    p.PlayMove(m);  // 3... O-O
    CHECK(p.white);
    CHECK(p.half_move_clock == 6);
    CHECK(p.full_move_count == 4);
    CHECK(int(p.d.bking_square) == g8);
    CHECK(int(p.d.bking)  == 1);  CHECK(int(p.d.bqueen) == 0);
    CHECK(p.at(e8) == ' ');  CHECK(p.at(g8) == 'k');
    CHECK(p.at(f8) == 'r');  CHECK(p.at(h8) == ' ');
}
