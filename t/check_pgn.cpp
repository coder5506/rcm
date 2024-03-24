#include "../src/chess/chess_game.h"
#include "doctest.h"

#include <cstdarg>
#include <cstring>

static void play_san_moves(Game& game, ...) {
    va_list args;
    va_start(args, game);
    const char* san = va_arg(args, const char*);
    while (san) {
        // game.play_san_move(san);
        san = va_arg(args, const char*);
    }
    va_end(args);
}

static const char* short_pgn(Game& game) {
    char* pgn   = (char*)""; // game_pgn(game);
    char* begin = strstr(pgn, "\n\n");
    return begin ? begin + 2 : pgn;
}

TEST_CASE("fool's mate") {
    Game g;
    play_san_moves(g, "f3", "e6", "g4", "Qh4", NULL);
    // ck_assert_str_eq(short_pgn(g), "1. f3 e6 2. g4 Qh4#");
}

TEST_CASE("fool's mate (list)") {
    Game g;
    play_san_moves(g, "f3", "e6", "g4", "Qh4", NULL);

    // struct List *list = game_pgn_list(g);
    // ck_assert_int_eq(list_length(list), 2);
    // ck_assert_str_eq((const char*)list_index(list, 0), "1. f3 e6");
    // ck_assert_str_eq((const char*)list_index(list, 1), "2. g4 Qh4#");
}

TEST_CASE("semi-Steinitz") {
    Game g;
    play_san_moves(g, "e4", "e5", "Nf3", "Nc6", "Bb5", "a6", "Ba4", "d6", NULL);
    // ck_assert_str_eq(short_pgn(g), "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 d6");

    // game_takeback(g); game_takeback(g); game_takeback(g);
    // play_san_moves(g, "d6", "d4", NULL);
    // ck_assert_str_eq(short_pgn(g), "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 (3... d6 4. d4) 4. Ba4 d6");
}

TEST_CASE("semi-Steinitz (list)") {
    Game g;
    play_san_moves(g, "e4", "e5", "Nf3", "Nc6", "Bb5", "a6", "Ba4", "d6", NULL);
    // game_takeback(g); game_takeback(g); game_takeback(g);

    // play_san_moves(g, "d6", NULL);
    // game_takeback(g);

    // struct List *list = game_pgn_list(g);
    // ck_assert_int_eq(list_length(list), 5);
    // ck_assert_str_eq((const char*)list_index(list, 0), "1. e4 e5");
    // ck_assert_str_eq((const char*)list_index(list, 1), "2. Nf3 Nc6");
    // ck_assert_str_eq((const char*)list_index(list, 2), "3. Bb5");
    // ck_assert_str_eq((const char*)list_index(list, 3), "3... a6");
    // ck_assert_str_eq((const char*)list_index(list, 4), "3... d6");
}

TEST_CASE("Steinitz") {
    Game g;
    play_san_moves(g, "e4", "e5", "Nf3", "Nc6", "Bb5", "d6", "d4", NULL);
    // ck_assert_str_eq(short_pgn(g), "1. e4 e5 2. Nf3 Nc6 3. Bb5 d6 4. d4");
}
