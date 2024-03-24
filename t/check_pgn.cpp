#include "../src/chess/chess_game.h"
#include "doctest.h"

#include <cstdarg>

using namespace std;

static void play_san_moves(Game& game, ...) {
    va_list args;
    va_start(args, game);
    const char* san = va_arg(args, const char*);
    while (san) {
        game.play_san_move(san);
        san = va_arg(args, const char*);
    }
    va_end(args);
}

static string short_pgn(const Game& game) {
    const auto pgn = game.pgn();
    const auto begin = pgn.find("\n\n");
    return begin == string::npos ? pgn : pgn.substr(begin + 2);
}

TEST_CASE("fool's mate") {
    Game g;
    play_san_moves(g, "f3", "e6", "g4", "Qh4", nullptr);
    CHECK(short_pgn(g) == "1. f3 e6 2. g4 Qh4#");
}

TEST_CASE("semi-Steinitz") {
    Game g;
    play_san_moves(g, "e4", "e5", "Nf3", "Nc6", "Bb5", "a6", "Ba4", "d6", nullptr);
    CHECK(short_pgn(g) == "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 d6");

    g.play_takeback(); g.play_takeback(); g.play_takeback();
    play_san_moves(g, "d6", "d4", nullptr);
    CHECK(short_pgn(g) == "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 (3... d6 4. d4) 4. Ba4 d6");
}

TEST_CASE("Steinitz") {
    Game g;
    play_san_moves(g, "e4", "e5", "Nf3", "Nc6", "Bb5", "d6", "d4", nullptr);
    CHECK(short_pgn(g) == "1. e4 e5 2. Nf3 Nc6 3. Bb5 d6 4. d4");
}
