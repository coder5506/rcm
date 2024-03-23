/****************************************************************************
 * ChessRules.cpp Chess classes - Rules of chess
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "../src/thc/ChessRules.h"
#include "doctest.h"

#include <iostream>

using namespace std;
using namespace thc;

// Adapted from ChessRules::TestInternals
TEST_CASE("test castling internals: rook moves") {
    ChessRules p;
    Move m;

    m.TerseIn(&p, "g1f3");
    p.PlayMove(m);
    m.TerseIn(&p, "g8f6");
    p.PlayMove(m);
    CHECK(int(p.d.wking)  == 1);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 1);
    CHECK(p.wking_allowed());
    CHECK(p.bking_allowed());
    CHECK(p.wqueen_allowed());
    CHECK(p.bqueen_allowed());

    // cout << p.squares << endl;
    m.TerseIn(&p, "h1g1");
    p.PlayMove(m);
    m.TerseIn(&p, "h8g8");
    p.PlayMove(m);
    m.TerseIn(&p, "g1h1");  // return of rook to h1 clears wking
    p.PlayMove(m);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 1);
    CHECK(!p.wking_allowed());
    CHECK(!p.bking_allowed());
    CHECK(p.wqueen_allowed());
    CHECK(p.bqueen_allowed());

    m.TerseIn(&p, "g8h8");  // return of rook to h8 clears bking
    p.PlayMove(m);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 0);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 1);
    CHECK(!p.wking_allowed());
    CHECK(!p.bking_allowed());
    CHECK(p.wqueen_allowed());
    CHECK(p.bqueen_allowed());

    m.TerseIn(&p, "b1c3");
    p.PlayMove(m);
    m.TerseIn(&p, "b8c6");
    p.PlayMove(m);
    m.TerseIn(&p, "a1b1");
    p.PlayMove(m);
    m.TerseIn(&p, "a8b8");
    p.PlayMove(m);
    m.TerseIn(&p, "b1a1");  // return of rook to a1 clears wqueen
    p.PlayMove(m);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 0);
    CHECK(int(p.d.wqueen) == 0);
    CHECK(int(p.d.bqueen) == 1);
    CHECK(!p.wking_allowed());
    CHECK(!p.bking_allowed());
    CHECK(!p.wqueen_allowed());
    CHECK(!p.bqueen_allowed());

    m.TerseIn(&p, "b8a8");  // return of rook to a8 clears bqueen
    p.PlayMove(m);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 0);
    CHECK(int(p.d.wqueen) == 0);
    CHECK(int(p.d.bqueen) == 0);
    CHECK(!p.wking_allowed());
    CHECK(!p.bking_allowed());
    CHECK(!p.wqueen_allowed());
    CHECK(!p.bqueen_allowed());
}

// Adapted from ChessRules::TestInternals
TEST_CASE("test castling internals: king moves") {
    ChessRules p;
    Move m;

    m.TerseIn(&p, "e2e3");
    p.PlayMove(m);
    m.TerseIn(&p, "e7e6");
    p.PlayMove(m);
    CHECK(int(p.d.wking)  == 1);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 1);
    CHECK(p.wking_allowed());
    CHECK(p.bking_allowed());
    CHECK(p.wqueen_allowed());
    CHECK(p.bqueen_allowed());

    m.TerseIn(&p, "e1e2");
    p.PlayMove(m);
    m.TerseIn(&p, "e8e7");
    p.PlayMove(m);
    m.TerseIn(&p, "e2e1");  // return of king to e1 clears wking, wqueen
    p.PlayMove(m);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 0);
    CHECK(int(p.d.bqueen) == 1);
    CHECK(!p.wking_allowed());
    CHECK(!p.bking_allowed());
    CHECK(!p.wqueen_allowed());
    CHECK(!p.bqueen_allowed());

    m.TerseIn(&p, "e7e8");
    p.PlayMove(m);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 0);
    CHECK(int(p.d.wqueen) == 0);
    CHECK(int(p.d.bqueen) == 0);
    CHECK(!p.wking_allowed());
    CHECK(!p.bking_allowed());
    CHECK(!p.wqueen_allowed());
    CHECK(!p.bqueen_allowed());
}

// Adapted from ChessRules::TestInternals
TEST_CASE("test internals") {
    ChessRules p;
    Move m;

    p.Forsyth("b3k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1");
    CHECK(p.white);
    CHECK(p.at(a8) == 'b');
    CHECK(p.half_move_clock == 0);
    CHECK(p.full_move_count == 1);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == e1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 1);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);


    m.TerseIn(&p, "h1h2");
    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == e1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 1);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PopMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == e1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 1);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == e1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 1);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);


    m.TerseIn(&p, "a8h1");
    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == e1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PopMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == e1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 1);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == e1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);


    m.TerseIn(&p, "e1c1");
    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == c1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PopMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == e1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == c1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);


    m.TerseIn(&p, "h1a8");
    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == c1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PopMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == c1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == c1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);


    m.TerseIn(&p, "c1b1");
    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == b1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PopMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == c1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == b1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);


    m.TerseIn(&p, "e8g8");
    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == b1);
    CHECK(int(p.d.bking_square) == g8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PopMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == b1);
    CHECK(int(p.d.bking_square) == e8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);

    p.PushMove(m);
    CHECK(int(p.d.enpassant_target) == SQUARE_INVALID);
    CHECK(int(p.d.wking_square) == b1);
    CHECK(int(p.d.bking_square) == g8);
    CHECK(int(p.d.wking)  == 0);
    CHECK(int(p.d.bking)  == 1);
    CHECK(int(p.d.wqueen) == 1);
    CHECK(int(p.d.bqueen) == 0);
}
