/****************************************************************************
 * ChessRules.cpp Chess classes - Rules of chess
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "../src/chess/chess_game.h"
#include "doctest.h"

#include <iostream>

using namespace std;
using namespace thc;

// Adapted from ChessRules::TestInternals
TEST_CASE("test castling internals: rook moves") {
    Game g;

    g.play_uci_move("g1f3");  // Nf3
    g.play_uci_move("g8f6");  // Nf6
    CHECK(g.d().wking());   CHECK(g.wking_allowed());
    CHECK(g.d().bking());   CHECK(g.bking_allowed());
    CHECK(g.d().wqueen());  CHECK(g.wqueen_allowed());
    CHECK(g.d().bqueen());  CHECK(g.bqueen_allowed());

    g.play_uci_move("h1g1");  // Rg1
    g.play_uci_move("h8g8");  // Rg8
    g.play_uci_move("g1h1");  // Rh1 -- return of rook to h1 clears wking
    CHECK(!g.d().wking());  CHECK(!g.wking_allowed());
    CHECK(g.d().bking());   CHECK(!g.bking_allowed());

    g.play_uci_move("g8h8");  // Rh8 -- return of rook to h8 clears bking
    CHECK(!g.d().wking());  CHECK(!g.wking_allowed());
    CHECK(!g.d().bking());  CHECK(!g.bking_allowed());

    g.play_uci_move("b1c3");  // Nc3
    g.play_uci_move("b8c6");  // Nc6
    g.play_uci_move("a1b1");  // Rb1
    g.play_uci_move("a8b8");  // Rb8
    g.play_uci_move("b1a1");  // Ra1 -- return of rook to a1 clears wqueen
    CHECK(!g.d().wqueen());  CHECK(!g.wqueen_allowed());
    CHECK(g.d().bqueen());   CHECK(!g.bqueen_allowed());

    g.play_uci_move("b8a8");  // Ra8 -- return of rook to a8 clears bqueen
    CHECK(!g.d().wqueen());  CHECK(!g.wqueen_allowed());
    CHECK(!g.d().bqueen());  CHECK(!g.bqueen_allowed());
}

// Adapted from ChessRules::TestInternals
TEST_CASE("test castling internals: king moves") {
    Game g;

    g.play_uci_move("e2e3");  // e3
    g.play_uci_move("e7e6");  // e6
    CHECK(g.d().wking());   CHECK(g.wking_allowed());
    CHECK(g.d().bking());   CHECK(g.bking_allowed());
    CHECK(g.d().wqueen());  CHECK(g.wqueen_allowed());
    CHECK(g.d().bqueen());  CHECK(g.bqueen_allowed());

    g.play_uci_move("e1e2");  // Ke2
    g.play_uci_move("e8e7");  // Ke7
    g.play_uci_move("e2e1");  // Ke1 -- return of king to e1 clears wking, wqueen
    CHECK(!g.d().wking());   CHECK(!g.wking_allowed());
    CHECK(g.d().bking());    CHECK(!g.bking_allowed());
    CHECK(!g.d().wqueen());  CHECK(!g.wqueen_allowed());
    CHECK(g.d().bqueen());   CHECK(!g.bqueen_allowed());

    g.play_uci_move("e7e8");  // Ke8
    CHECK(!g.d().wking());   CHECK(!g.wking_allowed());
    CHECK(!g.d().bking());   CHECK(!g.bking_allowed());
    CHECK(!g.d().wqueen());  CHECK(!g.wqueen_allowed());
    CHECK(!g.d().bqueen());  CHECK(!g.bqueen_allowed());
}

// Adapted from ChessRules::TestInternals
TEST_CASE("test internals") {
    Game g;

    g.fen("b3k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1");
    CHECK(g.WhiteToPlay());
    CHECK(g.half_move_clock() == 0);
    CHECK(g.full_move_count() == 1);
    CHECK(g.current()->d.enpassant_target == SQUARE_INVALID);
    CHECK(g.d().wking_square == e1);
    CHECK(g.d().bking_square == e8);
    CHECK(g.d().wking());  CHECK(g.d().wqueen());
    CHECK(g.d().bking());  CHECK(!g.d().bqueen());
    CHECK(g.wking_allowed());


    auto m = g.uci_move("h1h2");  // 1. Rh2
    g.play_move(m);
    CHECK(g.BlackToPlay());
    CHECK(g.half_move_clock() == 1);
    CHECK(g.full_move_count() == 1);
    CHECK(g.d().wking());    CHECK(!g.wking_allowed());
    CHECK(g.at(h1) == ' ');  CHECK(g.at(h2) == 'R');

    g.play_takeback(m);  // takeback 1. Rh2
    CHECK(g.WhiteToPlay());
    CHECK(g.d().wking());    CHECK(g.wking_allowed());
    CHECK(g.at(h1) == 'R');  CHECK(g.at(h2) == ' ');

    g.play_move(m);  // 1. Rh2
    CHECK(g.BlackToPlay());
    CHECK(g.half_move_clock() == 1);
    CHECK(g.full_move_count() == 1);
    CHECK(g.d().wking());    CHECK(!g.wking_allowed());
    CHECK(g.at(h1) == ' ');  CHECK(g.at(h2) == 'R');


    m = g.uci_move("a8h1");  // 1... Bh1
    g.play_move(m);
    CHECK(g.WhiteToPlay());
    CHECK(g.half_move_clock() == 2);
    CHECK(g.full_move_count() == 2);
    CHECK(!g.d().wking());
    CHECK(g.at(a8) == ' ');  CHECK(g.at(h1) == 'b');

    g.play_takeback(m);  // takeback 1... Bh1
    CHECK(g.BlackToPlay());
    CHECK(g.d().wking());
    CHECK(g.at(a8) == 'b');  CHECK(g.at(h1) == ' ');

    g.play_move(m);  // 1... Bh1
    CHECK(g.WhiteToPlay());
    CHECK(g.half_move_clock() == 2);
    CHECK(g.full_move_count() == 2);
    CHECK(!g.d().wking());
    CHECK(g.at(a8) == ' ');  CHECK(g.at(h1) == 'b');


    m = g.uci_move("e1c1");  // 2. O-O-O
    cout << int(m.src) << ", " << int(m.dst) << ", " << m.special << "\n";
    g.play_move(m);
    CHECK(g.BlackToPlay());
    CHECK(g.half_move_clock() == 3);
    CHECK(g.full_move_count() == 2);
    CHECK(g.d().wking_square == c1);
    CHECK(!g.d().wking());   CHECK(g.d().wqueen());
    CHECK(g.at(a1) == ' ');  CHECK(g.at(d1) == 'R');
    CHECK(g.at(c1) == 'K');  CHECK(g.at(e1) == ' ');

    g.play_takeback(m);  // takeback 2. O-O-O
    CHECK(g.WhiteToPlay());
    CHECK(g.d().wking_square == e1);
    CHECK(!g.d().wking());   CHECK(g.d().wqueen());
    CHECK(g.at(a1) == 'R');  CHECK(g.at(d1) == ' ');
    CHECK(g.at(c1) == ' ');  CHECK(g.at(e1) == 'K');

    g.play_move(m);  // 2. O-O-O
    CHECK(g.BlackToPlay());
    CHECK(g.half_move_clock() == 3);
    CHECK(g.full_move_count() == 2);
    CHECK(g.d().wking_square == c1);
    CHECK(!g.d().wking());   CHECK(g.d().wqueen());
    CHECK(g.at(a1) == ' ');  CHECK(g.at(d1) == 'R');
    CHECK(g.at(c1) == 'K');  CHECK(g.at(e1) == ' ');


    m = g.uci_move("h1a8");  // 2... Ba8
    g.play_move(m);
    CHECK(g.WhiteToPlay());
    CHECK(g.half_move_clock() == 4);
    CHECK(g.full_move_count() == 3);
    CHECK(g.at(a8) == 'b');  CHECK(g.at(h1) == ' ');

    g.play_takeback(m);  // takeback 2... Ba8
    CHECK(g.BlackToPlay());
    CHECK(g.at(a8) == ' ');  CHECK(g.at(h1) == 'b');

    g.play_move(m);  // 2... Ba8
    CHECK(g.WhiteToPlay());
    CHECK(g.half_move_clock() == 4);
    CHECK(g.full_move_count() == 3);
    CHECK(g.at(b1) == ' ');  CHECK(g.at(c1) == 'K');


    m = g.uci_move("c1b1");  // 3. Kb1
    g.play_move(m);
    CHECK(g.BlackToPlay());
    CHECK(g.half_move_clock() == 5);
    CHECK(g.full_move_count() == 3);
    CHECK(g.d().wking_square == b1);
    CHECK(!g.d().wking());   CHECK(g.d().wqueen());
    CHECK(g.at(b1) == 'K');  CHECK(g.at(c1) == ' ');

    g.play_takeback(m);  // takeback 3. Kb1
    CHECK(g.WhiteToPlay());
    CHECK(g.d().wking_square == c1);
    CHECK(!g.d().wking());   CHECK(g.d().wqueen());
    CHECK(g.at(b1) == ' ');  CHECK(g.at(c1) == 'K');

    g.play_move(m);  // 3. Kb1
    CHECK(g.BlackToPlay());
    CHECK(g.half_move_clock() == 5);
    CHECK(g.full_move_count() == 3);
    CHECK(g.d().wking_square == b1);
    CHECK(!g.d().wking());   CHECK(g.d().wqueen());
    CHECK(g.at(b1) == 'K');  CHECK(g.at(c1) == ' ');


    m = g.uci_move("e8g8");  // 3... O-O
    g.play_move(m);
    CHECK(g.WhiteToPlay());
    CHECK(g.half_move_clock() == 6);
    CHECK(g.full_move_count() == 4);
    CHECK(g.d().bking_square == g8);
    CHECK(g.d().bking());    CHECK(!g.d().bqueen());
    CHECK(g.at(e8) == ' ');  CHECK(g.at(g8) == 'k');
    CHECK(g.at(f8) == 'r');  CHECK(g.at(h8) == ' ');

    g.play_takeback(m);  // takeback 3... O-O
    CHECK(g.BlackToPlay());
    CHECK(g.d().bking_square == e8);
    CHECK(g.d().bking());    CHECK(!g.d().bqueen());
    CHECK(g.at(e8) == 'k');  CHECK(g.at(g8) == ' ');
    CHECK(g.at(f8) == ' ');  CHECK(g.at(h8) == 'r');

    g.play_move(m);  // 3... O-O
    CHECK(g.WhiteToPlay());
    CHECK(g.half_move_clock() == 6);
    CHECK(g.full_move_count() == 4);
    CHECK(g.d().bking_square == g8);
    CHECK(g.d().bking());    CHECK(!g.d().bqueen());
    CHECK(g.at(e8) == ' ');  CHECK(g.at(g8) == 'k');
    CHECK(g.at(f8) == 'r');  CHECK(g.at(h8) == ' ');
}
