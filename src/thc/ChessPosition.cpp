/****************************************************************************
 * ChessPosition.cpp Chess classes - Representation of the position on the board
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

/* Some project notes */
/*====================*/

/****************************************************************************
 * Class Hierarchy:
 *
 *  The Chess classes are intended to provide a reusable chess subsystem. To
 *  understand the hierarchy of classes, think in terms of the Java concept
 *  of derived classes "extending" base classes.
 *
 *  So
 *    ChessPosition
 *      ;A simple C style structure that holds a chess position
 *    ChessRules      extends ChessPosition
 *      ;ChessRules adds all the rules of standard chess
 ****************************************************************************/

/****************************************************************************
 * Explanation of namespace thc:
 *
 *  The Chess classes use the C++ namespace facility to ensure they can be
 *  used without name conflicts with other 3rd party code. A short, but
 *  highly likely to be unique namespace name is best, we choose "thc" which
 *  is short for TripleHappyChess.
 ****************************************************************************/

/****************************************************************************
 * My original license, now replaced by MIT license full text of which is
 * in file LICENSE in project's root directory.
 *
 * Licensing provisions for all TripleHappy Chess source code;
 *
 * Start Date: 15 February 2003.
 * This software is licensed to be used freely. The following licensing
 * provisions apply;
 *
 * 1) The 'author' is asserted to be the original author of the code, Bill
 *    Forster of Wellington, New Zealand.
 * 2) The 'licensee' is anyone else who wishes to use the software.
 * 3) The 'licensee' is entitled to do anything they wish with the software
 *    EXCEPT for any action that attempts to restrict in any way the rights
 *    granted in 4) below.
 * 4) The 'author' is entitled to do anything he wishes with the software.
 *
 * The intent of this license is to allow the licensees wide freedom to
 * incorporate, modify and sell the software, with the single caveat that
 * they cannot prevent the author from either further development or future
 * commercial use of the software.
 ****************************************************************************/

#include "ChessPosition.h"
#include "Move.h"
#include "MoveGen.h"
#include "PrivateChessDefs.h"
#include "fen.h"
#include "san.h"
#include "uci.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

using namespace std;
using namespace thc;


bool operator==(const ChessPosition& lhs, const ChessPosition& rhs) {
    return lhs.white == rhs.white &&
        lhs.half_move_clock == rhs.half_move_clock &&
        lhs.full_move_count == rhs.full_move_count &&
        lhs.d == rhs.d &&
        memcmp(lhs.squares, rhs.squares, sizeof lhs.squares) == 0;
}


ChessPosition::ChessPosition()
{
    memcpy(squares,
        "rnbqkbnr"
        "pppppppp"
        "        "
        "        "
        "        "
        "        "
        "PPPPPPPP"
        "RNBQKBNR", sizeof squares);
}


Square ChessPosition::groomed_enpassant_target() const {
    auto ret = SQUARE_INVALID;
    if (white && a6 <= d.enpassant_target && d.enpassant_target <= h6) {
        auto zap = true;  // zap unless there is a 'P' in place
        auto idx = d.enpassant_target + 8; //idx = SOUTH(d.enpassant_target)
        if (d.enpassant_target > a6 && squares[idx-1] == 'P') {
            zap = false;    // eg a5xb6 ep, through g5xh6 ep
        }
        if (d.enpassant_target < h6 && squares[idx+1] == 'P') {
            zap = false;    // eg b5xa6 ep, through h5xg6 ep
        }
        if (!zap) {
            ret = d.enpassant_target;
        }
    }
    else if (!white && a3 <= d.enpassant_target && d.enpassant_target <= h3) {
        bool zap = true;  // zap unless there is a 'p' in place
        auto idx = d.enpassant_target - 8; //idx = NORTH(d.enpassant_target)
        if (d.enpassant_target > a3 && squares[idx-1] == 'p') {
            zap = false;    // eg a4xb3 ep, through g4xh3 ep
        }
        if (d.enpassant_target < h3 && squares[idx+1] == 'p') {
            zap = false;    // eg b4xa3 ep, through h4xg3 ep
        }
        if (!zap) {
            ret = d.enpassant_target;
        }
    }
    return ret;
}


vector<Move> ChessPosition::legal_moves() const {
    return thc::GenLegalMoveList(*this);
}


bool ChessPosition::Evaluate() const {
    // Enemy king is attacked and our move, position is illegal
    const auto enemy_king = white ? d.bking_square : d.wking_square;
    return !thc::AttackedPiece(*this, enemy_king);
}


bool ChessPosition::Evaluate(const Move& move) const {
    return play_move(move).Evaluate();
}


ChessPosition ChessPosition::play_move(const Move& move) const {
    ChessPosition result{*this};
    result.apply_move(move);
    return result;
}


ChessPosition ChessPosition::play_san_move(string_view san_move) const {
    return play_move(this->san_move(san_move));
}


ChessPosition ChessPosition::play_uci_move(string_view uci_move) const {
    return play_move(this->uci_move(uci_move));
}


void ChessPosition::apply_move(const Move& move) {
    // Update full move count
    if (!white) {
        ++full_move_count;
    }

    // Update half move clock
    if (squares[move.src] == 'P' || squares[move.src] == 'p') {
        half_move_clock = 0;   // pawn move
    }
    else if (!IsEmptySquare(move.capture)) {
        half_move_clock = 0;   // capture
    }
    else {
        ++half_move_clock;    // neither pawn move nor capture
    }

    // Update castling prohibited flags for destination square, eg h8 -> bking
    switch (move.dst) {
    case a8: d.bqueen(false); break;
    case e8: d.bqueen(false);
    case h8: d.bking(false);  break;
    case a1: d.wqueen(false); break;
    case e1: d.wqueen(false);
    case h1: d.wking(false);  break;
    default:    // IMPORTANT - only dst is required since we also qualify
        break;  // castling with presence of rook and king on right squares.
    }           // (i.e., If a rook or king leaves its original square, the
                // castling prohibited flag is unaffected, but it doesn't matter
                // since we won't castle unless rook and king are present on the
                // right squares. If subsequently a king or rook returns, that's
                // okay too because the castling flag is cleared by its arrival
                // on the move.dst square, so castling remains prohibited).
    d.enpassant_target = SQUARE_INVALID;

    // Special handling might be required
    switch (move.special) {
    default:
        squares[move.dst] = squares[move.src];
        squares[move.src] = ' ';
        break;

    // King move updates king position in details field
    case SPECIAL_KING_MOVE:
        squares[move.dst] = squares[move.src];
        squares[move.src] = ' ';
        if (white) {
            d.wking_square = move.dst;
        }
        else {
            d.bking_square = move.dst;
        }
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_QUEEN:
        squares[move.src] = ' ';
        squares[move.dst] = (white?'Q':'q');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_ROOK:
        squares[move.src] = ' ';
        squares[move.dst] = (white?'R':'r');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_BISHOP:
        squares[move.src] = ' ';
        squares[move.dst] = (white?'B':'b');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_KNIGHT:
        squares[move.src] = ' ';
        squares[move.dst] = (white?'N':'n');
        break;

    // White enpassant removes pawn south of destination
    case SPECIAL_WEN_PASSANT:
        squares[move.src] = ' ';
        squares[move.dst] = 'P';
        squares[SOUTH(move.dst)] = ' ';
        break;

    // Black enpassant removes pawn north of destination
    case SPECIAL_BEN_PASSANT:
        squares[move.src] = ' ';
        squares[move.dst] = 'p';
        squares[NORTH(move.dst)] = ' ';
        break;

    // White pawn advances 2 squares sets an enpassant target
    case SPECIAL_WPAWN_2SQUARES:
        squares[move.src] = ' ';
        squares[move.dst] = 'P';
        d.enpassant_target = SOUTH(move.dst);
        break;

    // Black pawn advances 2 squares sets an enpassant target
    case SPECIAL_BPAWN_2SQUARES:
        squares[move.src] = ' ';
        squares[move.dst] = 'p';
        d.enpassant_target = NORTH(move.dst);
        break;

    // Castling moves update 4 squares each
    case SPECIAL_WK_CASTLING:
        squares[e1] = ' ';
        squares[f1] = 'R';
        squares[g1] = 'K';
        squares[h1] = ' ';
        d.wking_square = g1;
        break;
    case SPECIAL_WQ_CASTLING:
        squares[e1] = ' ';
        squares[d1] = 'R';
        squares[c1] = 'K';
        squares[a1] = ' ';
        d.wking_square = c1;
        break;
    case SPECIAL_BK_CASTLING:
        squares[e8] = ' ';
        squares[f8] = 'r';
        squares[g8] = 'k';
        squares[h8] = ' ';
        d.bking_square = g8;
        break;
    case SPECIAL_BQ_CASTLING:
        squares[e8] = ' ';
        squares[d8] = 'r';
        squares[c8] = 'k';
        squares[a8] = ' ';
        d.bking_square = c8;
        break;
    }

    // Toggle who-to-move
    white = !white;
}


/****************************************************************************
 * Set up position on board from Forsyth string with extensions
 *   return bool okay
 ****************************************************************************/
bool ChessPosition::Forsyth(const char* txt) {
    try {
        *this = fen::to_position(txt);
        return true;
    }
    catch (const std::exception& e) {
        return false;
    }
}


string ChessPosition::fen() const {
    return fen::from_position(*this);
}


Move ChessPosition::uci_move(string_view uci_move) const {
    return uci::to_move(*this, uci_move);
}


string ChessPosition::move_uci(const Move& move) const {
    return uci::from_move(move);
}


// Read natural string move eg "Nf3"
Move ChessPosition::san_move(string_view san_move) const {
    return san::to_move(*this, san_move);
}


string ChessPosition::move_san(const Move& move) const {
    return san::from_move(*this, move);
}
