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
    const Square src;
    const Square dst;
    SPECIAL special{NOT_SPECIAL};
    char    capture{' '};  // ' ' (empty) if move not a capture

    Move(Square src, Square dst, SPECIAL special = NOT_SPECIAL, char capture = ' ')
        : src{src}, dst{dst}, special{special}, capture{capture} {}

    Move& operator=(const Move& other) {
        const_cast<Square&>(src) = other.src;
        const_cast<Square&>(dst) = other.dst;
        special = other.special;
        capture = other.capture;
        return *this;
    }

    // Construct from UCI string
    explicit Move(std::string_view uci);

    // Describe in UCI notation
    std::string uci() const;

    bool is_capture() const { return capture != ' '; }
    bool is_promotion() const {
        return SPECIAL_PROMOTION_QUEEN <= special && special <= SPECIAL_PROMOTION_KNIGHT;
    }
};

// Move is a lightweight type, it is accommodated in only 32 bits
static_assert(sizeof(Move) == sizeof(unsigned int));

inline bool operator==(const Move& lhs, const Move& rhs) {
    return reinterpret_cast<const unsigned int&>(lhs) == reinterpret_cast<const unsigned int&>(rhs);
}

inline bool operator!=(const Move& lhs, const Move& rhs) {
    return reinterpret_cast<const unsigned int&>(lhs) != reinterpret_cast<const unsigned int&>(rhs);
}

}

#endif
