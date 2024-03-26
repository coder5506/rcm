#include "../src/chess/chess_game.h"
#include "doctest.h"

using namespace std;
using namespace thc;

static const Bitmap START = 0xFFFF00000000FFFF;

inline Bitmap lift(Bitmap boardstate, Square square) {
    return boardstate & ~(1ULL << square);
}

inline Bitmap place(Bitmap boardstate, Square square) {
    return boardstate | (1ULL << square);
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
    CHECK(p->move_san(moves.at( 0)) == "a3");
    CHECK(p->move_san(moves.at( 1)) == "a4");
    CHECK(p->move_san(moves.at( 2)) == "b3");
    CHECK(p->move_san(moves.at( 3)) == "b4");
    CHECK(p->move_san(moves.at( 4)) == "c3");
    CHECK(p->move_san(moves.at( 5)) == "c4");
    CHECK(p->move_san(moves.at( 6)) == "d3");
    CHECK(p->move_san(moves.at( 7)) == "d4");
    CHECK(p->move_san(moves.at( 8)) == "e3");
    CHECK(p->move_san(moves.at( 9)) == "e4");
    CHECK(p->move_san(moves.at(10)) == "f3");
    CHECK(p->move_san(moves.at(11)) == "f4");
    CHECK(p->move_san(moves.at(12)) == "g3");
    CHECK(p->move_san(moves.at(13)) == "g4");
    CHECK(p->move_san(moves.at(14)) == "h3");
    CHECK(p->move_san(moves.at(15)) == "h4");
    CHECK(p->move_san(moves.at(16)) == "Na3");
    CHECK(p->move_san(moves.at(17)) == "Nc3");
    CHECK(p->move_san(moves.at(18)) == "Nf3");
    CHECK(p->move_san(moves.at(19)) == "Nh3");
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
    optional<Move> takeback;

    auto maybe_valid = g.read_move(move(START, e2, e4), ActionList{}, candidates, takeback);
    auto p = const_cast<Position*>(g.current().get());
    CHECK(maybe_valid);
    CHECK(candidates.size() == 1);
    CHECK(p->move_san(candidates.at(0)) == "e4");
    CHECK(!takeback.has_value());
}

TEST_CASE("read 1... e5 from bitmap") {
    Game g;
    g.play_uci_move("e2e4");

    const auto boardstate = move(move(START, e2, e4), e7, e5);
    MoveList candidates;
    optional<Move> takeback;

    auto maybe_valid = g.read_move(boardstate, ActionList{}, candidates, takeback);
    auto p = const_cast<Position*>(g.current().get());
    CHECK(maybe_valid);
    CHECK(candidates.size() == 1);
    CHECK(p->move_san(candidates.at(0)) == "e5");
    CHECK(!takeback.has_value());
}

TEST_CASE("read takeback") {
    Game g;
    g.play_uci_move("e2e4");
    g.play_uci_move("e7e5");

    MoveList candidates;
    optional<Move> takeback;

    auto maybe_valid = g.read_move(move(START, e2, e4), ActionList{}, candidates, takeback);
    auto p = const_cast<Position*>(g.previous().get());
    CHECK(maybe_valid);
    CHECK(candidates.empty());
    CHECK(takeback.has_value());
    CHECK(p->move_san(*takeback) == "e5");
}

TEST_CASE("read incomplete move") {
    Game g;
    MoveList candidates;
    optional<Move> takeback;

    auto maybe_valid = g.read_move(lift(START, g1), ActionList{}, candidates, takeback);
    CHECK(maybe_valid);
    CHECK(candidates.empty());
    CHECK(!takeback.has_value());
}

TEST_CASE("read illegal move") {
    Game g;
    MoveList candidates;
    optional<Move> takeback;
    CHECK(!g.read_move(lift(START, f1), ActionList{}, candidates, takeback));
}

TEST_CASE("read out-of-turn move") {
    Game g;
    MoveList candidates;
    optional<Move> takeback;
    CHECK(!g.read_move(lift(START, e7), ActionList{}, candidates, takeback));
}
