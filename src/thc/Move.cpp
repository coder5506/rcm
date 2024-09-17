/****************************************************************************
 * Move.cpp Chess classes - Move
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "Move.h"
#include "uci.h"

#include <stdexcept>

using namespace std;
using namespace thc;

Move::Move(string_view uci_move)
    : src{SQ(uci_move[0], uci_move[1])},
      dst{SQ(uci_move[2], uci_move[3])}
{
    if (uci_move.size() < 4 ||
        uci_move[0] < 'a' || 'h' < uci_move[0] ||
        uci_move[1] < '1' || '8' < uci_move[1] ||
        uci_move[2] < 'a' || 'h' < uci_move[2] ||
        uci_move[3] < '1' || '8' < uci_move[3])
    {
        throw std::domain_error("Invalid UCI move: " + string(uci_move));
    }

    switch (uci_move[4]) {
    case 'q': special = SPECIAL_PROMOTION_QUEEN;  break;
    case 'r': special = SPECIAL_PROMOTION_ROOK;   break;
    case 'b': special = SPECIAL_PROMOTION_BISHOP; break;
    case 'n': special = SPECIAL_PROMOTION_KNIGHT; break;
    default:
        break;
    }
}

string Move::uci() const {
    return uci::from_move(*this);
}
