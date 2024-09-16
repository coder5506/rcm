/****************************************************************************
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef DETAIL_H
#define DETAIL_H

#include "Castling.h"
#include "ChessDefs.h"

namespace thc {

class DETAIL {
public:
    Square   enpassant_target{SQUARE_INVALID};
    Square   wking_square{e1};
    Square   bking_square{e8};
    Castling castling;

    bool wking()  const { return castling.wking(); }
    bool wqueen() const { return castling.wqueen(); }
    bool bking()  const { return castling.bking(); }
    bool bqueen() const { return castling.bqueen(); }

    void wking(bool val)  { castling.wking(val); }
    void wqueen(bool val) { castling.wqueen(val); }
    void bking(bool val)  { castling.bking(val); }
    void bqueen(bool val) { castling.bqueen(val); }
};

static_assert(sizeof(DETAIL) == sizeof(unsigned int));

inline bool eq_castling(const DETAIL& lhs, const DETAIL& rhs) {
    return lhs.castling == rhs.castling;
}

inline bool operator==(const DETAIL& lhs, const DETAIL& rhs) {
    return reinterpret_cast<const unsigned int&>(lhs) == reinterpret_cast<const unsigned int&>(rhs);
}

inline bool operator!=(const DETAIL& lhs, const DETAIL& rhs) {
    return reinterpret_cast<const unsigned int&>(lhs) != reinterpret_cast<const unsigned int&>(rhs);
}

}

#endif
