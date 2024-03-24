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
    union {
        struct {
            Square  src       : 8;
            Square  dst       : 8;
            SPECIAL special   : 8;
            int     capture   : 8;  // ' ' (empty) if move not a capture
        };
        std::uint32_t raw;
    };

    Move(Square src, Square dst, SPECIAL special = NOT_SPECIAL, int capture = ' ');
    explicit Move(std::string_view uci);

    std::string uci() const;

    bool is_promotion() const {
        return SPECIAL_PROMOTION_QUEEN <= special && special <= SPECIAL_PROMOTION_KNIGHT;
    }
};

inline bool operator==(const Move& lhs, const Move& rhs) {
    return lhs.raw == rhs.raw;
}

inline bool operator!=(const Move& lhs, const Move& rhs) {
    return lhs.raw != rhs.raw;
}

}

#endif
