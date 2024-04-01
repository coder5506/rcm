#include "doctest.h"
#include "../src/chess/chess_position.h"

using namespace std;
using namespace thc;

inline Action l(Square sq) { return Action{sq, SQUARE_INVALID}; }  // Lift
inline Action p(Square sq) { return Action{SQUARE_INVALID, sq}; }  // Place

TEST_CASE("opera game") {
    string pgn =
        "1. e4 e5 2. Nf3 d6 3. d4 Bg4 4. dxe5 Bxf3 5. Qxf3 dxe5 6. Bc4 Nf6 7. Qb3 Qe7 "
        "8. Nc3 c6 9. Bg5 b5 10. Nxb5 cxb5 11. Bxb5+ Nbd7 12. O-O-O Rd8 13. Rxd7 Rxd7 "
        "14. Rd1 Qe6 15. Bxd7+ Nxd7 16. Qb8+ Nxb8 17. Rd8#";

    ActionList actions{
        l(e2), p(e4),                // 1. e4
        l(e7), p(e5),                // 1...      e5
        l(g1), p(f3),                // 2. Nf3
        l(d7), p(d6),                // 2...      d6
        l(d2), p(d4),                // 3. d4
        l(c8), p(g4),                // 3...      Bg4
        l(e5), l(d4), p(e5),         // 4. dxe5
        l(f3), l(g4), p(f3),         // 4...      Bxf3
        l(f3), l(d1), p(f3),         // 5. Qxf3
        l(e5), l(d6), p(e5),         // 5...      dxe5
        l(f1), p(c4),                // 6. Bc4
        l(g8), p(f6),                // 6...      Nf6
        l(f3), p(b3),                // 7. Qb3
        l(d8), p(e7),                // 7...      Qe7
        l(b1), p(c3),                // 8. Nc3
        l(c7), p(c6),                // 8...      c6
        l(c1), p(g5),                // 9.  Bg5
        l(b7), p(b5),                // 9...      b5
        l(b5), l(c3), p(b5),         // 10. Nxb5
        l(b5), l(c6), p(b5),         // 10...      cxb5
        l(b5), l(c4), p(b5),         // 11. Bxb5+
        l(b8), p(d7),                // 11...      Nbd7
        l(e1), l(a1), p(d1), p(c1),  // 12. O-O-O
        l(a8), p(d8),                // 12...      Rd8
        l(d7), l(d1), p(d7),         // 13. Rxd7
        l(d7), l(d8), p(d8),         // 13...      Rxd7
        l(h1), p(d1),                // 14. Rd1
        l(e7), p(e6),                // 14...      Qe6
        l(d7), l(b5), p(d7),         // 15. Bxd7+
        l(d7), l(f6), p(d7),         // 15...      Nxd7
        l(b3), p(b8),                // 16. Qb8+
        l(b8), l(d7), p(b8),         // 16...      Nxb8
        l(d1), p(d8),                // 17. Rd8#
    };

    // TODO intent of this test is reconstruct entire game from actions, as in
    // `history_read_move` in centaur.cpp

    // Not implemented yet b/c centaur.cpp is too tighly coupled to the board,
    // and will need refactoring in order to test
}
