/****************************************************************************
 * demo.cpp Simple demo of THC Chess library
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "../src/chess/chess_game.h"
#include "../src/thc/gen.h"
#include "doctest.h"

using namespace std;
using namespace thc;

// Adapted from THC demo.cpp
TEST_CASE("test Italian opening") {
    Game g;

    g.play_san_move("e4");
    g.play_san_move("e5");
    g.play_san_move("Nf3");
    g.play_san_move("Nc6");
    g.play_san_move("Bc4");
    g.play_san_move("Bc5");
    CHECK(g.fen() == "r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");

    vector<Move> moves;
    vector<bool> check;
    vector<bool> mate;
    vector<bool> stalemate;

    auto p = g.current();
    gen::GenLegalMoveList(*p, moves, check, mate, stalemate);

    CHECK(moves.size() == 33);
    CHECK(p->move_san(moves.at( 0)) == "Bb3");
    CHECK(p->move_san(moves.at( 1)) == "Bb5");
    CHECK(p->move_san(moves.at( 2)) == "Ba6");
    CHECK(p->move_san(moves.at( 3)) == "Bd5");
    CHECK(p->move_san(moves.at( 4)) == "Be6");
    CHECK(p->move_san(moves.at( 5)) == "Bxf7+");
    CHECK(check.at( 5));
    CHECK(p->move_san(moves.at( 6)) == "Bd3");
    CHECK(p->move_san(moves.at( 7)) == "Be2");
    CHECK(p->move_san(moves.at( 8)) == "Bf1");
    CHECK(p->move_san(moves.at( 9)) == "Nd4");
    CHECK(p->move_san(moves.at(10)) == "Nxe5");
    CHECK(p->move_san(moves.at(11)) == "Nh4");
    CHECK(p->move_san(moves.at(12)) == "Ng1");
    CHECK(p->move_san(moves.at(13)) == "Ng5");
    CHECK(p->move_san(moves.at(14)) == "a3");
    CHECK(p->move_san(moves.at(15)) == "a4");
    CHECK(p->move_san(moves.at(16)) == "b3");
    CHECK(p->move_san(moves.at(17)) == "b4");
    CHECK(p->move_san(moves.at(18)) == "c3");
    CHECK(p->move_san(moves.at(19)) == "d3");
    CHECK(p->move_san(moves.at(20)) == "d4");
    CHECK(p->move_san(moves.at(21)) == "g3");
    CHECK(p->move_san(moves.at(22)) == "g4");
    CHECK(p->move_san(moves.at(23)) == "h3");
    CHECK(p->move_san(moves.at(24)) == "h4");
    CHECK(p->move_san(moves.at(25)) == "Na3");
    CHECK(p->move_san(moves.at(26)) == "Nc3");
    CHECK(p->move_san(moves.at(27)) == "Qe2");
    CHECK(p->move_san(moves.at(28)) == "Kf1");
    CHECK(p->move_san(moves.at(29)) == "Ke2");
    CHECK(p->move_san(moves.at(30)) == "O-O");
    CHECK(p->move_san(moves.at(31)) == "Rg1");
    CHECK(p->move_san(moves.at(32)) == "Rf1");
}

// Adapted from THC demo.cpp
TEST_CASE("test fool's mate") {
    Game g;

    CHECK(g.fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    auto m = g.current()->uci_move("g2g4");
    CHECK(g.current()->move_san(m) == "g4");
    g.play_move(m);
    CHECK(g.fen() == "rnbqkbnr/pppppppp/8/8/6P1/8/PPPPPP1P/RNBQKBNR b KQkq g3 0 1");

    m = g.current()->uci_move("e7e5");
    CHECK(g.current()->move_san(m) == "e5");
    g.play_move(m);
    CHECK(g.fen() == "rnbqkbnr/pppp1ppp/8/4p3/6P1/8/PPPPPP1P/RNBQKBNR w KQkq e6 0 2");

    m = g.current()->uci_move("f2f3");
    CHECK(g.current()->move_san(m) == "f3");
    g.play_move(m);
    CHECK(g.fen() == "rnbqkbnr/pppp1ppp/8/4p3/6P1/5P2/PPPPP2P/RNBQKBNR b KQkq - 0 2");

    TERMINAL eval_penultimate_position;
    auto legal1 = gen::Evaluate(*g.current(), eval_penultimate_position);
    CHECK(legal1);
    CHECK(eval_penultimate_position == NOT_TERMINAL);

    m = g.current()->uci_move("d8h4");
    CHECK(g.current()->move_san(m) == "Qh4#");
    g.play_move(m);
    CHECK(g.fen() == "rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3");

    TERMINAL eval_final_position;
    auto legal2 = gen::Evaluate(*g.current(), eval_final_position);
    CHECK(legal2);
    CHECK(eval_final_position == TERMINAL_WCHECKMATE);
}
