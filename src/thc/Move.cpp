/****************************************************************************
 * Move.cpp Chess classes - Move
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "Move.h"

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
        throw std::invalid_argument("Invalid UCI move: " + string(uci_move));
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
    // null move should be "0000" according to UCI spec
    char tmove[6] = {0};
    if (src != dst) {
        tmove[0] = FILE(src);
        tmove[1] = RANK(src);
        tmove[2] = FILE(dst);
        tmove[3] = RANK(dst);
        switch (special) {
        case SPECIAL_PROMOTION_QUEEN:  tmove[4] = 'q'; break;
        case SPECIAL_PROMOTION_ROOK:   tmove[4] = 'r'; break;
        case SPECIAL_PROMOTION_BISHOP: tmove[4] = 'b'; break;
        case SPECIAL_PROMOTION_KNIGHT: tmove[4] = 'n'; break;
        default:
            break;
        }
    }
    return tmove;
}
