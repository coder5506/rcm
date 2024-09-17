/****************************************************************************
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "gen.h"
#include "ChessPosition.h"

#include "PrivateChessDefs.h"

using namespace std;
using namespace thc;

// Generate moves for white pown
static void
WhitePawnMoves(const ChessPosition& position, Square square, vector<Move>& moves) {
    const auto* ptr = pawn_white_lookup[square];
    auto promotion = RANK(square) == '7';

    // Capture ray
    for (auto nbr_moves = *ptr++; nbr_moves != 0; --nbr_moves) {
        const Square dst = static_cast<Square>(*ptr++);
        if (dst == position.d.enpassant_target) {
            moves.push_back({square, dst, SPECIAL_WEN_PASSANT, 'p'});
        }
        else if (IsBlack(position.squares[dst])) {
            const auto capture = position.squares[dst];
            if (!promotion) {
                moves.push_back({square, dst, NOT_SPECIAL, capture});
            }
            else {
                moves.push_back({square, dst, SPECIAL_PROMOTION_QUEEN,  capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_KNIGHT, capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_BISHOP, capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_ROOK,   capture});
            }
        }
    }

    // Advance ray
    auto nbr_moves = *ptr++;
    for (auto i = 0; i < nbr_moves; ++i) {
        const Square dst = static_cast<Square>(*ptr++);

        // If square occupied, end now
        if (!IsEmptySquare(position.squares[dst])) {
            break;
        }
        if (!promotion) {
            moves.push_back({square, dst, i == 0 ? NOT_SPECIAL : SPECIAL_WPAWN_2SQUARES, ' '});
        }
        else {
            moves.push_back({square, dst, SPECIAL_PROMOTION_QUEEN,  ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_KNIGHT, ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_BISHOP, ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_ROOK,   ' '});
        }
    }
}

// Generate moves for black pown
static void
BlackPawnMoves(const ChessPosition& position, Square square, vector<Move>& moves) {
    const auto* ptr = pawn_black_lookup[square];
    auto promotion = RANK(square) == '2';

    // Capture ray
    for (auto nbr_moves = *ptr++; nbr_moves != 0; --nbr_moves) {
        const Square dst = static_cast<Square>(*ptr++);
        if (dst == position.d.enpassant_target) {
            moves.push_back({square, dst, SPECIAL_BEN_PASSANT, 'P'});
        }
        else if (IsWhite(position.squares[dst])) {
            const auto capture = position.squares[dst];
            if (!promotion) {
                moves.push_back({square, dst, NOT_SPECIAL, capture});
            }
            else {
                moves.push_back({square, dst, SPECIAL_PROMOTION_QUEEN,  capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_KNIGHT, capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_BISHOP, capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_ROOK,   capture});
            }
        }
    }

    // Advance ray
    auto nbr_moves = *ptr++;
    for (auto i = 0; i < nbr_moves; ++i) {
        const Square dst = static_cast<Square>(*ptr++);

        // If square occupied, end now
        if (!IsEmptySquare(position.squares[dst])) {
            break;
        }
        if (!promotion) {
            moves.push_back({square, dst, i == 0 ? NOT_SPECIAL : SPECIAL_BPAWN_2SQUARES, ' '});
        }
        else {
            moves.push_back({square, dst, SPECIAL_PROMOTION_QUEEN,  ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_KNIGHT, ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_BISHOP, ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_ROOK,   ' '});
        }
    }
}

// Generate moves for pieces that move along single move rays (N, K)
static void
ShortMoves(
    const ChessPosition& position,
    Square square,
    const lte* ptr,
    SPECIAL special,
    vector<Move>& moves)
{
    for (auto nbr_moves = *ptr++; nbr_moves != 0; --nbr_moves) {
        const Square dst = static_cast<Square>(*ptr++);
        const auto piece = position.squares[dst];

        // If square not occupied (empty), add move to list
        if (IsEmptySquare(piece)) {
            moves.push_back({square, dst, special, ' '});
        }
        // Else if occupied by enemy man, add move to list as a capture
        else if ((position.white && IsBlack(piece)) || (!position.white && IsWhite(piece))) {
            moves.push_back({square, dst, special, piece});
        }
    }
}

// Generate moves for pieces that move along multi-move rays (B, R, Q)
static void
LongMoves(
    const ChessPosition& position, Square square, const lte* ptr, vector<Move>& moves)
{
    for (auto nbr_rays = *ptr++; nbr_rays != 0; --nbr_rays) {
        for (auto ray_len = *ptr++; ray_len != 0; --ray_len) {
            const Square dst = static_cast<Square>(*ptr++);
            const auto piece = position.squares[dst];

            // If square not occupied (empty), add move to list
            if (IsEmptySquare(piece)) {
                moves.push_back({square, dst, NOT_SPECIAL, ' '});
            }
            // Else must move to end of ray
            else {
                ptr += ray_len - 1;

                // If not occupied by our man add a capture
                if ((position.white && IsBlack(piece)) || (!position.white && IsWhite(piece))) {
                    moves.push_back({square, dst, NOT_SPECIAL, piece});
                }
                break;
            }
        }
    }
}

// Is a square is attacked by enemy?
bool gen::AttackedSquare(const ChessPosition& position, Square square, bool enemy_is_white) {
    Square dst;
    const auto* ptr = (enemy_is_white ? attacks_black_lookup[square] : attacks_white_lookup[square] );
    auto nbr_rays = *ptr++;
    while (nbr_rays--) {
        auto ray_len = *ptr++;
        while (ray_len--) {
            dst = static_cast<Square>(*ptr++);
            const auto piece = position.squares[dst];

            // If square not occupied (empty), continue
            if (IsEmptySquare(piece)) {
                ptr++;  // skip mask
            }
            // Else if occupied
            else {
                lte mask = *ptr++;

                // White attacker?
                if (IsWhite(piece) && enemy_is_white) {
                    if (to_mask[piece] & mask) {
                        return true;
                    }
                }
                // Black attacker ?
                else if (IsBlack(piece) && !enemy_is_white) {
                    if (to_mask[piece] & mask) {
                        return true;
                    }
                }

                // Goto end of ray
                ptr += 2 * ray_len;
                ray_len = 0;
            }
        }
    }

    ptr = knight_lookup[square];
    auto nbr_squares = *ptr++;
    while (nbr_squares--) {
        dst = static_cast<Square>(*ptr++);
        const auto piece = position.squares[dst];

        // If occupied by an enemy knight, we have found an attacker
        if ((enemy_is_white && piece == 'N') || (!enemy_is_white && piece == 'n')) {
            return true;
        }
    }
    return false;
}

// Generate list of king moves
void gen::KingMoves(const ChessPosition& position, Square square, vector<Move>& moves) {
    const auto* ptr = king_lookup[square];
    ShortMoves(position, square, ptr, SPECIAL_KING_MOVE, moves);

    // White castling
    if (square == e1)   // king on e1 ?
    {
        // King side castling
        if (position.squares[g1] == ' ' &&
            position.squares[f1] == ' ' &&
            position.squares[h1] == 'R' &&
            position.d.wking()          &&
            !AttackedSquare(position, e1, false) &&
            !AttackedSquare(position, f1, false) &&
            !AttackedSquare(position, g1, false))
        {
            moves.push_back({e1, g1, SPECIAL_WK_CASTLING, ' '});
        }

        // Queen side castling
        if (position.squares[b1] == ' ' &&
            position.squares[c1] == ' ' &&
            position.squares[d1] == ' ' &&
            position.squares[a1] == 'R' &&
            position.d.wqueen()         &&
            !AttackedSquare(position, e1, false) &&
            !AttackedSquare(position, d1, false) &&
            !AttackedSquare(position, c1, false))
        {
            moves.push_back({e1, c1, SPECIAL_WQ_CASTLING, ' '});
        }
    }

    // Black castling
    if (square == e8)   // king on e8 ?
    {
        // King side castling
        if (position.squares[g8] == ' ' &&
            position.squares[f8] == ' ' &&
            position.squares[h8] == 'r' &&
            position.d.bking()          &&
            !AttackedSquare(position, e8, true) &&
            !AttackedSquare(position, f8, true) &&
            !AttackedSquare(position, g8, true))
        {
            moves.push_back({e8, g8, SPECIAL_BK_CASTLING, ' '});
        }

        // Queen side castling
        if (position.squares[b8] == ' ' &&
            position.squares[c8] == ' ' &&
            position.squares[d8] == ' ' &&
            position.squares[a8] == 'r' &&
            position.d.bqueen()         &&
            !AttackedSquare(position, e8, true) &&
            !AttackedSquare(position, d8, true) &&
            !AttackedSquare(position, c8, true))
        {
            moves.push_back({e8, c8, SPECIAL_BQ_CASTLING, ' '});
        }
    }
}

vector<Move> gen::GenMoveList(const ChessPosition& position) {
    vector<Move> moves;

    for (auto square = a8; square <= h1; ++square) {
        // If square occupied by a piece of the right colour
        const auto piece = position.squares[square];
        if ((position.white && IsBlack(piece)) || (!position.white && IsWhite(piece))) {
            continue;
        }

        // Generate moves according to the occupying piece
        switch (piece) {
        case 'P':
            WhitePawnMoves(position, square, moves);
            break;
        case 'p':
            BlackPawnMoves(position, square, moves);
            break;
        case 'N': case 'n':
            ShortMoves(position, square, knight_lookup[square], NOT_SPECIAL, moves);
            break;
        case 'B': case 'b':
            LongMoves(position, square, bishop_lookup[square], moves);
            break;
        case 'R': case 'r':
            LongMoves(position, square, rook_lookup[square], moves);
            break;
        case 'Q': case 'q':
            LongMoves(position, square, queen_lookup[square], moves);
            break;
        case 'K': case 'k':
            KingMoves(position, square, moves);
            break;
        }
    }

    return moves;
}

// Determine if an occupied square is attacked
bool gen::AttackedPiece(const ChessPosition& position, Square square) {
    const bool enemy_is_white = IsBlack(position.squares[square]);
    return AttackedSquare(position, square, enemy_is_white);
}

vector<Move> gen::GenLegalMoveList(const ChessPosition& position) {
    vector<Move> result;
    for (auto move : GenMoveList(position)) {
        if (position.Evaluate(move)) {
            result.push_back(move);
        }
    }
    return result;
}

bool gen::Evaluate(const ChessPosition& position, TERMINAL& score_terminal) {
    score_terminal = NOT_TERMINAL;

    // Enemy king is attacked and our move, position is illegal
    if (!position.Evaluate()) {
        return false;
    }

    // Work out if the game is over by checking for any legal moves
    const auto any = GenLegalMoveList(position).size();

    // If no legal moves, position is either checkmate or stalemate
    if (any == 0) {
        if (AttackedPiece(position, position.king_square())) {
            score_terminal = (position.white ? TERMINAL_WCHECKMATE : TERMINAL_BCHECKMATE);
        }
        else {
            score_terminal = (position.white ? TERMINAL_WSTALEMATE : TERMINAL_BSTALEMATE);
        }
    }

    return true;
}

// Create a list of all legal moves in this position, with extra info
void gen::GenLegalMoveList(
    const ChessPosition& position,
    vector<Move>& moves,
    vector<bool>& check,
    vector<bool>& mate,
    vector<bool>& stalemate)
{
    moves.clear();
    check.clear();
    mate.clear();
    stalemate.clear();

    // Loop copying the proven good ones
    for (auto move : GenMoveList(position)) {
        const auto after = position.play_move(move);

        TERMINAL terminal_score;
        const bool okay = Evaluate(after, terminal_score);
        const bool bcheck = AttackedPiece(after, after.king_square());

        if (okay) {
            moves.push_back(move);
            stalemate.push_back(
                terminal_score == TERMINAL_WSTALEMATE ||
                terminal_score == TERMINAL_BSTALEMATE);
            const bool is_mate =
                terminal_score == TERMINAL_WCHECKMATE ||
                terminal_score == TERMINAL_BCHECKMATE;
            mate.push_back(is_mate);
            check.push_back(is_mate ? false : bcheck);
        }
    }
}
