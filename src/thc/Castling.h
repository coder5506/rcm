/****************************************************************************
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef CASTLING_H
#define CASTLING_H

namespace thc {

class Castling {
private:
    enum {
        W_KING  = 1,
        W_QUEEN = 2,
        B_KING  = 4,
        B_QUEEN = 8
    };
    unsigned char flags;

public:
    Castling() : flags{0x0F} {}

    bool wking()  const { return flags & W_KING; }
    bool wqueen() const { return flags & W_QUEEN; }
    bool bking()  const { return flags & B_KING; }
    bool bqueen() const { return flags & B_QUEEN; }

    void wking(bool val)  { if (val) flags |= W_KING;  else flags &= ~W_KING; }
    void wqueen(bool val) { if (val) flags |= W_QUEEN; else flags &= ~W_QUEEN; }
    void bking(bool val)  { if (val) flags |= B_KING;  else flags &= ~B_KING; }
    void bqueen(bool val) { if (val) flags |= B_QUEEN; else flags &= ~B_QUEEN; }

    friend bool operator==(const Castling& lhs, const Castling& rhs);
    friend bool operator!=(const Castling& lhs, const Castling& rhs);
};

static_assert(sizeof(Castling) == sizeof(unsigned char));

inline bool operator==(const Castling& lhs, const Castling& rhs) {
    return lhs.flags == rhs.flags;
}

inline bool operator!=(const Castling& lhs, const Castling& rhs) {
    return lhs.flags != rhs.flags;
}

}

#endif
