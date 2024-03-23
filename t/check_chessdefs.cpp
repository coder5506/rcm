#include "../src/thc/ChessDefs.h"
#include "doctest.h"

using namespace thc;

TEST_CASE("test square increment") {
    Square sq = a8;
    CHECK(sq++ == a8);
    CHECK(sq++ == b8);
    CHECK(sq   == c8);
    CHECK(++sq == d8);
    CHECK(++sq == e8);
    CHECK(sq   == e8);
}

TEST_CASE("test square utilities") {
    CHECK(SQ('c', '5') == c5);
    CHECK(IFILE(c5) == 2);
    CHECK(IRANK(c5) == 4);
    CHECK(FILE(c5)  == 'c');
    CHECK(RANK(c5)  == '5');

    CHECK(SOUTH(c5) == c4);
    CHECK(NORTH(c5) == c6);
    CHECK(SW(c5) == b4);
    CHECK(SE(c5) == d4);
    CHECK(NW(c5) == b6);
    CHECK(NE(c5) == d6);
    CHECK(get_file(c5) == 'c');
    CHECK(get_rank(c5) == '5');
    CHECK(make_square('c', '5') == c5);

    CHECK(FILE(a1) == 'a');
    CHECK(FILE(h8) == 'h');
    CHECK(RANK(a1) == '1');
    CHECK(RANK(h8) == '8');
}
