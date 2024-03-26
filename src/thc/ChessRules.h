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
    bool IsLegal(ILLEGAL_REASON& reason);

    Move uci_move(std::string_view uci_move);
    Move san_move(std::string_view san_move);
    std::string move_uci(Move move);
    std::string move_san(Move move);

    // Play a move
    void PlayMove(Move imove);
    void play_san_move(std::string_view san_move);
    void play_uci_move(std::string_view uci_move);

    // Check draw rules (50 move rule etc.)
    bool IsDraw(bool white_asks, DRAWTYPE& result);

    // Get number of times position has been repeated
    int GetRepetitionCount();

    // Check insufficient material draw rule
    bool IsInsufficientDraw(bool white_asks, DRAWTYPE& result);

    // Evaluate a position, returns bool okay (not okay means illegal position)
    bool Evaluate();
    bool Evaluate(TERMINAL& score_terminal);

    bool is_legal();
    bool is_legal(Move);

    // Is a square is attacked by enemy ?
    bool AttackedSquare(Square square, bool enemy_is_white);

    // Determine if an occupied square is attacked
    bool AttackedPiece(Square square);

    // Create a list of all legal moves in this position
    void GenLegalMoveList(MoveList& moves);
    MoveList GenLegalMoveList();

    //  Create a list of all legal moves in this position, with extra info
    void GenLegalMoveList(MoveList&          moves,
                          std::vector<bool>& check,
                          std::vector<bool>& mate,
                          std::vector<bool>& stalemate);

    // Make a move (with the potential to undo)
    void PushMove(Move m);

    // Undo a move
    void PopMove(Move m);

protected:
    MoveList select_legal(const MoveList&);

    // Generate a list of all possible moves in a position (including
    //  illegally "moving into check")
    void GenMoveList(MoveList& moves);
    MoveList GenMoveList();

    // Generate moves for pieces that move along multi-move rays (B,R,Q)
    void LongMoves(MoveList& moves, Square square, const lte* ptr);

    // Generate moves for pieces that move along single-move rays (K,N,P)
    void ShortMoves(MoveList& moves, Square square, const lte* ptr, SPECIAL special);

    // Generate list of king moves
    void KingMoves(MoveList& moves, Square square);

    // Generate list of white pawn moves
    void WhitePawnMoves(MoveList& moves, Square square);

    // Generate list of black pawn moves
    void BlackPawnMoves(MoveList& moves, Square square);

    // Evaluate a position, returns bool okay (not okay means illegal position)
    bool Evaluate(MoveList *p, TERMINAL& score_terminal);
};

}

#endif
