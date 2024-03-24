/****************************************************************************
 * demo.cpp Simple demo of THC Chess library
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "../src/thc/ChessRules.h"
#include "doctest.h"

using namespace std;
using namespace thc;

// Adapted from THC demo.cpp
TEST_CASE("test Italian opening") {
    ChessRules p;

    p.play_san_move("e4");
    p.play_san_move("e5");
    p.play_san_move("Nf3");
    p.play_san_move("Nc6");
    p.play_san_move("Bc4");
    p.play_san_move("Bc5");
    CHECK(p.fen() == "r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4");

    vector<Move> moves;
    vector<bool> check;
    vector<bool> mate;
    vector<bool> stalemate;
    p.GenLegalMoveList(moves, check, mate, stalemate);

    CHECK(moves.size() == 33);
    CHECK(moves.at( 0).NaturalOut(&p) == "Bb3");
    CHECK(moves.at( 1).NaturalOut(&p) == "Bb5");
    CHECK(moves.at( 2).NaturalOut(&p) == "Ba6");
    CHECK(moves.at( 3).NaturalOut(&p) == "Bd5");
    CHECK(moves.at( 4).NaturalOut(&p) == "Be6");
    CHECK(moves.at( 5).NaturalOut(&p) == "Bxf7+");
    CHECK(check.at( 5));
    CHECK(moves.at( 6).NaturalOut(&p) == "Bd3");
    CHECK(moves.at( 7).NaturalOut(&p) == "Be2");
    CHECK(moves.at( 8).NaturalOut(&p) == "Bf1");
    CHECK(moves.at( 9).NaturalOut(&p) == "Nd4");
    CHECK(moves.at(10).NaturalOut(&p) == "Nxe5");
    CHECK(moves.at(11).NaturalOut(&p) == "Nh4");
    CHECK(moves.at(12).NaturalOut(&p) == "Ng1");
    CHECK(moves.at(13).NaturalOut(&p) == "Ng5");
    CHECK(moves.at(14).NaturalOut(&p) == "a3");
    CHECK(moves.at(15).NaturalOut(&p) == "a4");
    CHECK(moves.at(16).NaturalOut(&p) == "b3");
    CHECK(moves.at(17).NaturalOut(&p) == "b4");
    CHECK(moves.at(18).NaturalOut(&p) == "c3");
    CHECK(moves.at(19).NaturalOut(&p) == "d3");
    CHECK(moves.at(20).NaturalOut(&p) == "d4");
    CHECK(moves.at(21).NaturalOut(&p) == "g3");
    CHECK(moves.at(22).NaturalOut(&p) == "g4");
    CHECK(moves.at(23).NaturalOut(&p) == "h3");
    CHECK(moves.at(24).NaturalOut(&p) == "h4");
    CHECK(moves.at(25).NaturalOut(&p) == "Na3");
    CHECK(moves.at(26).NaturalOut(&p) == "Nc3");
    CHECK(moves.at(27).NaturalOut(&p) == "Qe2");
    CHECK(moves.at(28).NaturalOut(&p) == "Kf1");
    CHECK(moves.at(29).NaturalOut(&p) == "Ke2");
    CHECK(moves.at(30).NaturalOut(&p) == "O-O");
    CHECK(moves.at(31).NaturalOut(&p) == "Rg1");
    CHECK(moves.at(32).NaturalOut(&p) == "Rf1");
}

// Adapted from THC demo.cpp
TEST_CASE("test fool's mate") {
    ChessRules p;
    Move m;

    CHECK(p.fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    m.TerseIn(&p, "g2g4");
    CHECK(m.NaturalOut(&p) == "g4");
    p.PlayMove(m);
    CHECK(p.fen() == "rnbqkbnr/pppppppp/8/8/6P1/8/PPPPPP1P/RNBQKBNR b KQkq g3 0 1");

    m.TerseIn(&p, "e7e5");
    CHECK(m.NaturalOut(&p) == "e5");
    p.PlayMove(m);
    CHECK(p.fen() == "rnbqkbnr/pppp1ppp/8/4p3/6P1/8/PPPPPP1P/RNBQKBNR w KQkq e6 0 2");

    m.TerseIn(&p, "f2f3");
    CHECK(m.NaturalOut(&p) == "f3");
    p.PlayMove(m);
    CHECK(p.fen() == "rnbqkbnr/pppp1ppp/8/4p3/6P1/5P2/PPPPP2P/RNBQKBNR b KQkq - 0 2");

    TERMINAL eval_penultimate_position;
    auto legal1 = p.Evaluate(eval_penultimate_position);
    CHECK(legal1);
    CHECK(eval_penultimate_position == NOT_TERMINAL);

    m.TerseIn(&p, "d8h4");
    CHECK(m.NaturalOut(&p) == "Qh4#");
    p.PlayMove(m);
    CHECK(p.fen() == "rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3");

    TERMINAL eval_final_position;
    auto legal2 = p.Evaluate(eval_final_position);
    CHECK(legal2);
    CHECK(eval_final_position == TERMINAL_WCHECKMATE);
}
