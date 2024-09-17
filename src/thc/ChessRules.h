/****************************************************************************
 * ChessRules.h Chess classes - Rules of chess
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef CHESSRULES_H
#define CHESSRULES_H

#include "ChessPosition.h"
#include "Move.h"

#include <vector>

namespace thc {

using MoveList = std::vector<Move>;

// Encapsulates state of game and operations available
class ChessRules: public ChessPosition {
private:
    MoveList            history;
    std::vector<DETAIL> detail_stack;

public:
    //  Test for legal position, sets reason to a mask of possibly multiple reasons
    bool IsLegal(ILLEGAL_REASON& reason) const;

    // Play a move
    void PlayMove(Move imove);

    // Check draw rules (50 move rule etc.)
    bool IsDraw(bool white_asks, DRAWTYPE& result) const;

    // Get number of times position has been repeated
    int GetRepetitionCount();

    // Check insufficient material draw rule
    bool IsInsufficientDraw(bool white_asks, DRAWTYPE& result) const;

    bool Evaluate() const { return ChessPosition::Evaluate(); }
    bool Evaluate(const Move& move) const { return ChessPosition::Evaluate(move); }

    // Evaluate a position, returns bool okay (not okay means illegal position)
    bool Evaluate(TERMINAL& score_terminal) const;

    // Is a square is attacked by enemy ?
    bool AttackedSquare(Square square, bool enemy_is_white) const;

    // Determine if an occupied square is attacked
    bool AttackedPiece(Square square) const;

    // Create a list of all legal moves in this position
    void GenLegalMoveList(MoveList& moves) const;
    MoveList GenLegalMoveList() const { return legal_moves(); }

    //  Create a list of all legal moves in this position, with extra info
    void GenLegalMoveList(MoveList&          moves,
                          std::vector<bool>& check,
                          std::vector<bool>& mate,
                          std::vector<bool>& stalemate) const;

    // Make a move (with the potential to undo)
    void PushMove(Move m);

    // Undo a move
    void PopMove(Move m);

protected:
    // Generate list of king moves
    void KingMoves(MoveList& moves, Square square) const;
};

}

#endif
