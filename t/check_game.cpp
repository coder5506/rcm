#include "../src/chess/chess_game.h"
#include "doctest.h"

using namespace thc;

static const Bitmap START = 0xffff00000000ffff;

inline Bitmap lift(Bitmap boardstate, Square square) {
    return boardstate & ~(1ull << square);
}

inline Bitmap place(Bitmap boardstate, Square square) {
    return boardstate | (1ull << square);
}

inline Bitmap move(Bitmap boardstate, Square from, Square to) {
    return place(lift(boardstate, from), to);
}

TEST_CASE("starting position") {
    Game g;

    CHECK(g.bitmap() == START);
    CHECK(g.fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    auto moves = g.legal_moves();
    CHECK(moves.size() == 20);

    auto p = const_cast<Position*>(g.current().get());
    CHECK(moves.at( 0).NaturalOut(p) == "a3");
    CHECK(moves.at( 1).NaturalOut(p) == "a4");
    CHECK(moves.at( 2).NaturalOut(p) == "b3");
    CHECK(moves.at( 3).NaturalOut(p) == "b4");
    CHECK(moves.at( 4).NaturalOut(p) == "c3");
    CHECK(moves.at( 5).NaturalOut(p) == "c4");
    CHECK(moves.at( 6).NaturalOut(p) == "d3");
    CHECK(moves.at( 7).NaturalOut(p) == "d4");
    CHECK(moves.at( 8).NaturalOut(p) == "e3");
    CHECK(moves.at( 9).NaturalOut(p) == "e4");
    CHECK(moves.at(10).NaturalOut(p) == "f3");
    CHECK(moves.at(11).NaturalOut(p) == "f4");
    CHECK(moves.at(12).NaturalOut(p) == "g3");
    CHECK(moves.at(13).NaturalOut(p) == "g4");
    CHECK(moves.at(14).NaturalOut(p) == "h3");
    CHECK(moves.at(15).NaturalOut(p) == "h4");
    CHECK(moves.at(16).NaturalOut(p) == "Na3");
    CHECK(moves.at(17).NaturalOut(p) == "Nc3");
    CHECK(moves.at(18).NaturalOut(p) == "Nf3");
    CHECK(moves.at(19).NaturalOut(p) == "Nh3");
}

TEST_CASE("play 1. e4 e5") {
    Game g;

    g.play_uci_move("e2e4");
    CHECK(g.bitmap() == move(START, e2, e4));
    CHECK(g.fen() == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    g.play_uci_move("e7e5");
    CHECK(g.bitmap() == move(move(START, e2, e4), e7, e5));
    CHECK(g.fen() == "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2");
}

TEST_CASE("read 1. e4 from bitmap") {
    Game g;
    MoveList candidates;
    std::optional<Move> takeback;
    auto maybe_valid = g.read_move(move(START, e2, e4), ActionList{}, candidates, takeback);

    auto p = const_cast<Position*>(g.current().get());
    CHECK(maybe_valid);
    CHECK(candidates.size() == 1);
    CHECK(candidates.at(0).NaturalOut(p) == "e4");
    CHECK(!takeback.has_value());
}

TEST_CASE("read 1... e5 from bitmap") {
    Game g;
    g.play_uci_move("e2e4");

    const auto boardstate = move(move(START, e2, e4), e7, e5);
    MoveList candidates;
    std::optional<Move> takeback;
    auto maybe_valid = g.read_move(boardstate, ActionList{}, candidates, takeback);

    auto p = const_cast<Position*>(g.current().get());
    CHECK(maybe_valid);
    CHECK(candidates.size() == 1);
    CHECK(candidates.at(0).NaturalOut(p) == "e5");
    CHECK(!takeback.has_value());
}

TEST_CASE("takeback") {
    Game g;
    g.play_uci_move("e2e4");
    g.play_uci_move("e7e5");

    MoveList candidates;
    std::optional<Move> takeback;
    auto maybe_valid = g.read_move(move(START, e2, e4), ActionList{}, candidates, takeback);

    auto p = const_cast<Position*>(g.current().get());
    // CHECK(maybe_valid);
    CHECK(candidates.empty());
    // CHECK(takeback.has_value());
    // CHECK(takeback.value().NaturalOut(p) == "e5");
}

TEST_CASE("illegal") {
    Game g;
    MoveList candidates;
    std::optional<Move> takeback;
    CHECK(!g.read_move(lift(START, f1), ActionList{}, candidates, takeback));
}

TEST_CASE("incomplete") {
    Game g;
    MoveList candidates;
    std::optional<Move> takeback;

    auto maybe_valid = g.read_move(lift(START, g1), ActionList{}, candidates, takeback);
    CHECK(maybe_valid);
    CHECK(candidates.empty());
    CHECK(!takeback.has_value());
}

TEST_CASE("out-of-turn") {
    Game g;
    MoveList candidates;
    std::optional<Move> takeback;
    CHECK(!g.read_move(lift(START, e7), ActionList{}, candidates, takeback));
}
