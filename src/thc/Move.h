/****************************************************************************
 * Move.h Chess classes - Move
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef MOVE_H
#define MOVE_H

#include "ChessDefs.h"

#include <string>

namespace thc {

class Move {
public:
    // Move is a lightweight type, it is accommodated in only 32 bits
    Square  src;
    Square  dst;
    SPECIAL special{NOT_SPECIAL};
    char    capture{' '};  // ' ' (empty) if move not a capture

    Move(Square src, Square dst, SPECIAL special = NOT_SPECIAL, char capture = ' ');
    explicit Move(std::string_view uci);

    std::string uci() const;

    bool is_promotion() const {
        return SPECIAL_PROMOTION_QUEEN <= special && special <= SPECIAL_PROMOTION_KNIGHT;
    }
};

static_assert(sizeof(Move) == sizeof(unsigned int));

inline bool operator==(const Move& lhs, const Move& rhs) {
    return reinterpret_cast<const unsigned int&>(lhs) == reinterpret_cast<const unsigned int&>(rhs);
}

inline bool operator!=(const Move& lhs, const Move& rhs) {
    return reinterpret_cast<const unsigned int&>(lhs) != reinterpret_cast<const unsigned int&>(rhs);
}

}

#endif
