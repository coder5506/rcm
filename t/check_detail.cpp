#include "../src/thc/ChessPosition.h"
#include "doctest.h"

using namespace thc;

TEST_CASE("test DETAIL") {
    DETAIL d1;
    DETAIL d2{d1};

    CHECK(d1 == d2);
    CHECK(eq_castling(d1, d2));

    d2.enpassant_target = a3;
    CHECK(d1 != d2);
    CHECK(eq_castling(d1, d2));

    d1.wking(false);
    CHECK(!eq_castling(d1, d2));
}
