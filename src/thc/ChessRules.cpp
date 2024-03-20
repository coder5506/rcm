/****************************************************************************
 * ChessRules.cpp Chess classes - Rules of chess
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "ChessRules.h"
#include "PrivateChessDefs.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace thc;

// Table indexed by Square, gives mask for DETAIL_CASTLING, such that a move
//  to (or from) that square results in castling being prohibited, eg a move
//  to e8 means that subsequently black kingside and black queenside castling
//  is prohibited
static unsigned char castling_prohibited_table[] =
{
    (unsigned char)(~BQUEEN), 0xff, 0xff, 0xff,                             // a8-d8
    (unsigned char)(~(BQUEEN+BKING)), 0xff, 0xff, (unsigned char)(~BKING),  // e8-h8
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                         // a7-h7
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                         // a6-h6
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                         // a5-h5
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                         // a4-h4
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                         // a3-h3
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,                         // a2-h2
    (unsigned char)(~WQUEEN), 0xff, 0xff, 0xff,                             // a1-d1
    (unsigned char)(~(WQUEEN+WKING)),  0xff, 0xff, (unsigned char)(~WKING)  // e1-h1
};

/****************************************************************************
 * Test internals, for porting to new environments etc
 *   For the moment at least, this is best used by stepping through it
 *   thoughtfully with a debugger. It is not set up to automatically
 *   check whether THC is going to work in the new environment
 ****************************************************************************/
static int log_discard( const char *, ... ) { return 0; }
bool ChessRules::TestInternals( int (*log)(const char *,...) )
{
    if( log == NULL )
        log = log_discard;
    Init();
    Move mv;
    log( "All castling allowed %08x\n", *DETAIL_ADDR );
    mv.TerseIn(this,"g1f3");
    PlayMove(mv);
    mv.TerseIn(this,"g8f6");
    PlayMove(mv);
    log( "All castling allowed %08x\n", *DETAIL_ADDR );
    bool KQkq_allowed   = wking && bking && wqueen && bqueen;
    bool KQkq_allowed_f = wking_allowed() && bking_allowed() && wqueen_allowed() && bqueen_allowed();
    mv.TerseIn(this,"h1g1");
    PlayMove(mv);
    mv.TerseIn(this,"h8g8");
    PlayMove(mv);
    mv.TerseIn(this,"g1h1");  // return of rook to h1 clears wking
    PlayMove(mv);
    log( "WKING castling not allowed %08x\n", *DETAIL_ADDR );
    bool Qkq_allowed  = !wking && bking && wqueen && bqueen;
    bool Qkq_allowed_f = !wking_allowed() && !bking_allowed() && wqueen_allowed() && bqueen_allowed();
    // The _f version is different, !bking_allowed() because it checks and finds no rook on h8
    mv.TerseIn(this,"g8h8");  // return of rook to h8 clears bking
    PlayMove(mv);
    log( "WKING BKING castling not allowed %08x\n", *DETAIL_ADDR );
    bool Qq_allowed   = !wking && !bking && wqueen && bqueen;
    bool Qq_allowed_f = !wking_allowed() && !bking_allowed() && wqueen_allowed() && bqueen_allowed();
    mv.TerseIn(this,"b1c3");
    PlayMove(mv);
    mv.TerseIn(this,"b8c6");
    PlayMove(mv);
    mv.TerseIn(this,"a1b1");
    PlayMove(mv);
    mv.TerseIn(this,"a8b8");
    PlayMove(mv);
    mv.TerseIn(this,"b1a1");  // return of rook to a1 clears wqueen
    PlayMove(mv);
    log( "WKING BKING WQUEEN castling not allowed %08x\n", *DETAIL_ADDR );
    bool q_allowed   = !wking && !bking && !wqueen && bqueen;
    bool q_allowed_f = !wking_allowed() && !bking_allowed() && !wqueen_allowed() && !bqueen_allowed();
    // The _f version is different, !bqueen_allowed() because it checks and finds no rook on a8
    mv.TerseIn(this,"b8a8");  // return of rook to a8 clears bqueen
    PlayMove(mv);
    log( "WKING BKING WQUEEN BQUEEN castling not allowed %08x\n", *DETAIL_ADDR );
    bool none_allowed   = !wking && !bking && !wqueen && !bqueen;
    bool none_allowed_f = !wking_allowed() && !bking_allowed() && !wqueen_allowed() && !bqueen_allowed();
    ChessPosition::Init();
    log( "All castling allowed %08x\n", *DETAIL_ADDR );
    mv.TerseIn(this,"e2e3");
    PlayMove(mv);
    mv.TerseIn(this,"e7e6");
    PlayMove(mv);
    log( "All castling allowed %08x\n", *DETAIL_ADDR );
    mv.TerseIn(this,"e1e2");
    PlayMove(mv);
    mv.TerseIn(this,"e8e7");
    PlayMove(mv);
    mv.TerseIn(this,"e2e1");  // return of king to e1 clears wking, wqueen
    PlayMove(mv);
    log( "WKING WQUEEN castling not allowed %08x\n", *DETAIL_ADDR );
    bool kq_allowed   = !wking && bking && !wqueen && bqueen;
    bool kq_allowed_f = !wking_allowed() && !bking_allowed() && !wqueen_allowed() && !bqueen_allowed();
    // The _f version is different, !bking_allowed() and !bqueen_allowed() because they check and finds no king on e8
    mv.TerseIn(this,"e7e8");
    PlayMove(mv);
    log( "WKING WQUEEN BKING BQUEEN castling not allowed %08x\n", *DETAIL_ADDR );
    bool none_allowed2   = !wking && !bking && !wqueen && !bqueen;
    bool none_allowed2_f = !wking_allowed() && !bking_allowed() && !wqueen_allowed() && !bqueen_allowed();
    const char *fen = "b3k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1";
    Move move;
    Forsyth(fen);
    log( "Addresses etc.;\n" );
    log( " this = 0x%p\n",                         this );
    log( " (void *)this = 0x%p\n",                 (void *)this );
    log( " &white = 0x%p\n",                       &white );
    log( " &squares[0] = 0x%p\n",                  &squares[0] );
    log( " &half_move_clock = 0x%p\n",             &half_move_clock );
    log( " &full_move_count = 0x%p\n",             &full_move_count );
    log( " size to end of full_move_count = %lu", ((char *)&full_move_count - (char *)this) + sizeof(full_move_count) );
    log( " sizeof(ChessPosition) = %lu (should be 4 more than size to end of full_move_count)\n",
           sizeof(ChessPosition) );
    log( " sizeof(Move) = %lu\n",                  sizeof(Move) );

    log( " sizeof(ChessPositionRaw) = %lu\n", sizeof(ChessPositionRaw) );
    log( " (offsetof(ChessPositionRaw,full_move_count) + sizeof(full_move_count) + sizeof(DETAIL) =");
    log( " %lu + %lu + %lu = %lu\n",
           offsetof(ChessPositionRaw,full_move_count), sizeof(full_move_count), sizeof(DETAIL),
           offsetof(ChessPositionRaw,full_move_count) + sizeof(full_move_count) + sizeof(DETAIL)
           );
    for( int i=0; i<6; i++ )
    {
        switch(i)
        {
            case 0: move.TerseIn(this,"h1h2");    break;
            case 1: move.TerseIn(this,"a8h1");    break;
            case 2: move.TerseIn(this,"e1c1");    break;
            case 3: move.TerseIn(this,"h1a8");    break;
            case 4: move.TerseIn(this,"c1b1");    break;
            case 5: move.TerseIn(this,"e8g8");    break;
        }
        unsigned char *p = (unsigned char *)DETAIL_ADDR;
        log( " DETAIL_ADDR = 0x%p\n",  p );
        log( " DETAIL_ADDR[0] = %02x\n",  p[0] );
        log( " DETAIL_ADDR[1] = %02x\n",  p[1] );
        log( " DETAIL_ADDR[2] = %02x\n",  p[2] );
        log( " DETAIL_ADDR[3] = %02x\n",  p[3] );
        log( "Before %s: enpassant_target=0x%02x, wking_square=0x%02x, bking_square=0x%02x,"
               " wking=%s, wqueen=%s, bking=%s, bqueen=%s\n",
               move.TerseOut().c_str(),
               enpassant_target,
               wking_square,
               bking_square,
               wking ?"true":"false",
               wqueen?"true":"false",
               bking ?"true":"false",
               bqueen?"true":"false" );
        PushMove(move);
        log( "After PushMove(): enpassant_target=0x%02x, wking_square=0x%02x, bking_square=0x%02x,"
               " wking=%s, wqueen=%s, bking=%s, bqueen=%s\n",
               enpassant_target,
               wking_square,
               bking_square,
               wking ?"true":"false",
               wqueen?"true":"false",
               bking ?"true":"false",
               bqueen?"true":"false" );
        PopMove(move);
        log( "After PopMove(): enpassant_target=0x%02x, wking_square=0x%02x, bking_square=0x%02x,"
               " wking=%s, wqueen=%s, bking=%s, bqueen=%s\n",
               enpassant_target,
               wking_square,
               bking_square,
               wking ?"true":"false",
               wqueen?"true":"false",
               bking ?"true":"false",
               bqueen?"true":"false" );
        PushMove(move);
        log( "After PushMove(): enpassant_target=0x%02x, wking_square=0x%02x, bking_square=0x%02x,"
               " wking=%s, wqueen=%s, bking=%s, bqueen=%s\n",
               enpassant_target,
               wking_square,
               bking_square,
               wking ?"true":"false",
               wqueen?"true":"false",
               bking ?"true":"false",
               bqueen?"true":"false" );
    }

    // Later, extend this to check addresses etc
    return KQkq_allowed && Qkq_allowed && Qq_allowed && q_allowed && none_allowed && kq_allowed && none_allowed2 &&
           KQkq_allowed_f && Qkq_allowed_f && Qq_allowed_f && q_allowed_f && none_allowed_f && kq_allowed_f && none_allowed2_f;
}

/****************************************************************************
 * Play a move
 ****************************************************************************/
void ChessRules::PlayMove(Move imove) {
    // Legal move - save it in history
    history[history_idx++] = imove; // ring array

    // Update full move count
    if (!white) {
        ++full_move_count;
    }

    // Update half move clock
    if (squares[imove.src] == 'P' || squares[imove.src] == 'p') {
        half_move_clock = 0;   // pawn move
    }
    else if (!IsEmptySquare(imove.capture)) {
        half_move_clock = 0;   // capture
    }
    else {
        ++half_move_clock;    // neither pawn move nor capture
    }

    // Actually play the move
    PushMove(imove);
}

// Create a list of all legal moves in this position
void ChessRules::GenLegalMoveList(vector<Move>& moves) {
    moves.clear();

    // Generate all moves, including illegal (e.g. put king in check) moves
    vector<Move> list2;
    GenMoveList(list2);

    // Loop copying the proven good ones
    for (auto move : list2) {
        PushMove(move);
        const bool okay = Evaluate();
        PopMove(move);
        if (okay) {
            moves.push_back(move);
        }
    }
}

// Create a list of all legal moves in this position, with extra info
void ChessRules::GenLegalMoveList(vector<Move>& moves,
                                  vector<bool>& check,
                                  vector<bool>& mate,
                                  vector<bool>& stalemate)
{
    moves.clear();
    check.clear();
    mate.clear();
    stalemate.clear();

    // Generate all moves, including illegal (e.g. put king in check) moves
    vector<Move> list2;
    GenMoveList(list2);

    // Loop copying the proven good ones
    for (auto move : list2) {
        PushMove(move);

        TERMINAL terminal_score;
        const bool okay = Evaluate(terminal_score);

        const Square king_to_move = static_cast<Square>(white ? wking_square : bking_square);
        const bool bcheck = AttackedPiece(king_to_move);
        PopMove(move);

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

/****************************************************************************
 * Check draw rules (50 move rule etc.)
 ****************************************************************************/
bool ChessRules::IsDraw(bool white_asks, DRAWTYPE& result) {
    bool   draw=false;

    // Insufficient mating material
    draw =  IsInsufficientDraw( white_asks, result );

    // 50 move rule
    if( !draw && half_move_clock>=100 )
    {
        result = DRAWTYPE_50MOVE;
        draw = true;
    }

    // 3 times repetition,
    if( !draw && GetRepetitionCount()>=3 )
    {
        result = DRAWTYPE_REPITITION;
        draw = true;
    }

    if( !draw )
        result = NOT_DRAW;
    return( draw );
}

/****************************************************************************
 * Get number of times position has been repeated
 ****************************************************************************/
int ChessRules::GetRepetitionCount() {
    int matches=0;

    //  Save those aspects of current position that are changed by multiple
    //  PopMove() calls as we search backwards (i.e. squares, white,
    //  detail, detail_idx)
    char save_squares[sizeof(squares)];
    memcpy( save_squares, squares, sizeof(save_squares) );
    unsigned char save_detail_idx = detail_idx;  // must be unsigned char
    bool          save_white      = white;
    unsigned char idx             = history_idx; // must be unsigned char
    DETAIL_SAVE;

    // Search backwards ....
    int nbr_half_moves = (full_move_count-1)*2 + (!white?1:0);
    if( nbr_half_moves > nbrof(history)-1 )
        nbr_half_moves = nbrof(history)-1;
    if( nbr_half_moves > nbrof(detail_stack)-1 )
        nbr_half_moves = nbrof(detail_stack)-1;
    for( int i=0; i<nbr_half_moves; i++ )
    {
        Move m = history[--idx];
        if( m.src == m.dst )
            break;  // unused history is set to zeroed memory
        PopMove(m);

        // ... looking for matching positions
        if( white    == save_white      && // quick ones first!
            DETAIL_EQ_KING_POSITIONS    &&
            0 == memcmp(squares,save_squares,sizeof(squares) )
            )
        {
            matches++;
            if( !DETAIL_EQ_ALL )    // Castling flags and/or enpassant target different?
            {
                // It might not be a match (but it could be - we have to unpack what the differences
                //  really mean)
                bool revoke_match = false;

                // Revoke match if different value of en-passant target square means different
                //  en passant possibilities
                if( !DETAIL_EQ_EN_PASSANT )
                {
                    int ep_saved = (int)(tmp&0xff);
                    int ep_now   = (int)(*DETAIL_ADDR&0xff);

                    // Work out whether each en_passant is a real one, i.e. is there an opposition
                    //  pawn in place to capture (if not it's just a double pawn advance with no
                    //  actual enpassant consequences)
                    bool real=false;
                    int ep = ep_saved;
                    char const *squ = save_squares;
                    for( int j=0; j<2; j++ )
                    {
                        if( ep == 0x10 )    // 0x10 = a6
                        {
                             real = (squ[SE(ep)] == 'P');
                        }
                        else if( 0x10<ep && ep<0x17 )   // 0x10 = h6
                        {
                             real = (squ[SW(ep)] == 'P' || squ[SE(ep)] == 'P');
                        }
                        else if( ep==0x17 )
                        {
                             real = (squ[SW(ep)] == 'P');
                        }
                        else if( 0x28==ep )   // 0x28 = a3
                        {
                             real = (squ[NE(ep)] == 'p');
                        }
                        else if( 0x28<ep && ep<0x2f )   // 0x2f = h3
                        {
                             real = (squ[NE(ep)] == 'p' || squ[NW(ep)] == 'p');
                        }
                        else if( ep==0x2f )
                        {
                             real = (squ[NW(ep)] == 'p' );
                        }
                        if( j > 0 )
                            ep_now = real?ep:0x40;      // evaluate second time through
                        else
                        {
                            ep_saved = real?ep:0x40;    // evaluate first time through
                            ep = ep_now;                // setup second time through
                            squ = squares;
                            real = false;
                        }
                    }

                    // If for example one en_passant is real and the other not, it's not a real match
                    if( ep_saved != ep_now )
                        revoke_match = true;
                }

                // Revoke match if different value of castling flags means different
                //  castling possibilities
                if( !revoke_match && !DETAIL_EQ_CASTLING )
                {
                    bool wking_saved  = save_squares[e1]=='K' && save_squares[h1]=='R' && (int)(tmp&(WKING<<24));
                    bool wking_now    = squares[e1]=='K' && squares[h1]=='R' && (int)(*DETAIL_ADDR&(WKING<<24));
                    bool bking_saved  = save_squares[e8]=='k' && save_squares[h8]=='r' && (int)(tmp&(BKING<<24));
                    bool bking_now    = squares[e8]=='k' && squares[h8]=='r' && (int)(*DETAIL_ADDR&(BKING<<24));
                    bool wqueen_saved = save_squares[e1]=='K' && save_squares[a1]=='R' && (int)(tmp&(WQUEEN<<24));
                    bool wqueen_now   = squares[e1]=='K' && squares[a1]=='R' && (int)(*DETAIL_ADDR&(WQUEEN<<24));
                    bool bqueen_saved = save_squares[e8]=='k' && save_squares[a8]=='r' && (int)(tmp&(BQUEEN<<24));
                    bool bqueen_now   = squares[e8]=='k' && squares[a8]=='r' && (int)(*DETAIL_ADDR&(BQUEEN<<24));
                    revoke_match = ( wking_saved != wking_now ||
                                     bking_saved != bking_now ||
                                     wqueen_saved != wqueen_now ||
                                     bqueen_saved != bqueen_now );
                }

                // If the real castling or enpassant possibilities differ, it's not a match
                //  At one stage we just did a naive binary match of the details - not good enough. For example
                //  a rook moving away from h1 doesn't affect the WKING flag, but does disallow white king side
                //  castling
                if( revoke_match )
                     matches--;
            }
        }

        // For performance reasons, abandon search early if pawn move
        //  or capture
        if( squares[m.src]=='P' || squares[m.src]=='p' || !IsEmptySquare(m.capture) )
            break;
    }

    // Restore current position
    memcpy( squares, save_squares, sizeof(squares) );
    white      = save_white;
    detail_idx = save_detail_idx;
    DETAIL_RESTORE;
    return( matches+1 );  // +1 counts original position
}

/****************************************************************************
 * Check insufficient material draw rule
 ****************************************************************************/
bool ChessRules::IsInsufficientDraw(bool white_asks, DRAWTYPE& result) {
    char   piece;
    int    piece_count=0;
    bool   bishop_or_knight=false, lone_wking=true, lone_bking=true;
    bool   draw=false;

    // Loop through the board
    for( Square square=a8; square<=h1; ++square )
    {
        piece = squares[square];
        switch( piece )
        {
            case 'B':
            case 'b':
            case 'N':
            case 'n':       bishop_or_knight=true;  // and fall through
            case 'Q':
            case 'q':
            case 'R':
            case 'r':
            case 'P':
            case 'p':       piece_count++;
                            if( isupper(piece) )
                                lone_wking = false;
                            else
                                lone_bking = false;
                            break;
        }
        if( !lone_wking && !lone_bking )
            break;  // quit early for performance
    }

    // Automatic draw if K v K or K v K+N or K v K+B
    //  (note that K+B v K+N etc. is not auto granted due to
    //   selfmates in the corner)
    if( piece_count==0 ||
        (piece_count==1 && bishop_or_knight)
      )
    {
        draw = true;
        result = DRAWTYPE_INSUFFICIENT_AUTO;
    }
    else
    {

        // Otherwise side playing against lone K can claim a draw
        if( white_asks && lone_bking )
        {
            draw   = true;
            result = DRAWTYPE_INSUFFICIENT;
        }
        else if( !white_asks && lone_wking )
        {
            draw   = true;
            result = DRAWTYPE_INSUFFICIENT;
        }
    }
    return( draw );
}

// Generate a list of all possible moves in a position
void ChessRules::GenMoveList(vector<Move>& moves) {
    // Convenient spot for some asserts
    //  Have a look at TestInternals() for this,
    //   A ChessPositionRaw should finish with 32 bits of detail information
    //   (see DETAIL macros, this assert() checks that)
    assert(
        sizeof(ChessPositionRaw) ==
               (offsetof(ChessPositionRaw,full_move_count) + sizeof(full_move_count) + sizeof(DETAIL))
    );

    // We also rely on Moves being 32 bits for the implementation of Move
    //  bitwise == and != operators
    assert(sizeof(Move) == sizeof(int32_t));

    // Clear move list
    moves.clear();

    // Loop through all squares
    for (Square square = a8; square <= h1; ++square) {
        // If square occupied by a piece of the right colour
        const char piece=squares[square];
        if ((white && IsBlack(piece)) || (!white && IsWhite(piece))) {
            continue;
        }

        // Generate moves according to the occupying piece
        switch (piece) {
        case 'P':
            WhitePawnMoves(moves, square );
            break;
        case 'p':
            BlackPawnMoves(moves, square );
            break;
        case 'N':
        case 'n': {
            const lte* ptr = knight_lookup[square];
            ShortMoves(moves, square, ptr, NOT_SPECIAL);
            break;
        }
        case 'B':
        case 'b': {
            const lte* ptr = bishop_lookup[square];
            LongMoves(moves, square, ptr);
            break;
        }
        case 'R':
        case 'r': {
            const lte* ptr = rook_lookup[square];
            LongMoves(moves, square, ptr);
            break;
        }
        case 'Q':
        case 'q': {
            const lte* ptr = queen_lookup[square];
            LongMoves(moves, square, ptr);
            break;
        }
        case 'K':
        case 'k':
            KingMoves(moves, square);
            break;
        }
    }
}

// Generate moves for pieces that move along multi-move rays (B,R,Q)
void ChessRules::LongMoves(vector<Move>& moves, Square square, const lte* ptr) {
    lte nbr_rays = *ptr++;
    while (--nbr_rays >= 0) {
        lte ray_len = *ptr++;
        while (--ray_len >= 0) {
            const Square dst = static_cast<Square>(*ptr++);
            const char piece = squares[dst];

            // If square not occupied (empty), add move to list
            if (IsEmptySquare(piece)) {
                moves.push_back({square, dst, NOT_SPECIAL, ' '});
            }
            // Else must move to end of ray
            else {
                ptr += ray_len;
                ray_len = 0;

                // If not occupied by our man add a capture
                if ((white && IsBlack(piece)) || (!white && IsWhite(piece))) {
                    moves.push_back({square, dst, NOT_SPECIAL, piece});
                }
            }
        }
    }
}

// Generate moves for pieces that move along single move rays (N,K)
void ChessRules::ShortMoves(
    vector<Move>& moves, Square square, const lte* ptr, SPECIAL special)
{
    lte nbr_moves = *ptr++;
    while (--nbr_moves >= 0) {
        const Square dst = static_cast<Square>(*ptr++);
        const char piece = squares[dst];

        // If square not occupied (empty), add move to list
        if (IsEmptySquare(piece)) {
            moves.push_back({square, dst, special, ' '});
        }
        // Else if occupied by enemy man, add move to list as a capture
        else if ((white && IsBlack(piece)) || (!white && IsWhite(piece))) {
            moves.push_back({square, dst, special, piece});
        }
    }
}

// Generate list of king moves
void ChessRules::KingMoves(vector<Move>& moves, Square square) {
    const lte* ptr = king_lookup[square];
    ShortMoves(moves, square, ptr, SPECIAL_KING_MOVE);

    // White castling
    if (square == e1)   // king on e1 ?
    {
        // King side castling
        if (squares[g1] == ' '        &&
            squares[f1] == ' '        &&
            squares[h1] == 'R'        &&
            (wking)                   &&
            !AttackedSquare(e1,false) &&
            !AttackedSquare(f1,false) &&
            !AttackedSquare(g1,false))
        {
            moves.push_back({e1, g1, SPECIAL_WK_CASTLING, ' '});
        }

        // Queen side castling
        if (squares[b1] == ' '        &&
            squares[c1] == ' '        &&
            squares[d1] == ' '        &&
            squares[a1] == 'R'        &&
            (wqueen)                  &&
            !AttackedSquare(e1,false) &&
            !AttackedSquare(d1,false) &&
            !AttackedSquare(c1,false))
        {
            moves.push_back({e1, c1, SPECIAL_WQ_CASTLING, ' '});
        }
    }

    // Black castling
    if (square == e8)   // king on e8 ?
    {
        // King side castling
        if (squares[g8] == ' '       &&
            squares[f8] == ' '       &&
            squares[h8] == 'r'       &&
            (bking)                  &&
            !AttackedSquare(e8,true) &&
            !AttackedSquare(f8,true) &&
            !AttackedSquare(g8,true))
        {
            moves.push_back({e8, g8, SPECIAL_BK_CASTLING, ' '});
        }

        // Queen side castling
        if (squares[b8] == ' '       &&
            squares[c8] == ' '       &&
            squares[d8] == ' '       &&
            squares[a8] == 'r'       &&
            (bqueen)                 &&
            !AttackedSquare(e8,true) &&
            !AttackedSquare(d8,true) &&
            !AttackedSquare(c8,true))
        {
            moves.push_back({e8, c8, SPECIAL_BQ_CASTLING, ' '});
        }
    }
}

// Generate list of white pawn moves
void ChessRules::WhitePawnMoves(vector<Move>& moves, Square square) {
    const lte* ptr = pawn_white_lookup[square];
    bool promotion = RANK(square) == '7';

    // Capture ray
    lte nbr_moves = *ptr++;
    while (--nbr_moves >= 0) {
        const Square dst = static_cast<Square>(*ptr++);
        if (dst == enpassant_target) {
            moves.push_back({square, dst, SPECIAL_WEN_PASSANT, 'p'});
        }
        else if (IsBlack(squares[dst])) {
            const int capture = squares[dst];
            if (!promotion) {
                moves.push_back({square, dst, NOT_SPECIAL, capture});
            }
            else {
                // Generate (under)promotions in the order (Q),N,B,R
                //  but we no longer rely on this elsewhere as it
                //  stops us reordering moves
                moves.push_back({square, dst, SPECIAL_PROMOTION_QUEEN,  capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_KNIGHT, capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_BISHOP, capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_ROOK,   capture});
            }
        }
    }

    // Advance ray
    nbr_moves = *ptr++;
    for (int i = 0; i < nbr_moves; ++i) {
        const Square dst = static_cast<Square>(*ptr++);

        // If square occupied, end now
        if (!IsEmptySquare(squares[dst])) {
            break;
        }
        if (!promotion) {
            moves.push_back({square, dst, i == 0 ? NOT_SPECIAL : SPECIAL_WPAWN_2SQUARES, ' '});
        }
        else {
            // Generate (under)promotions in the order (Q),N,B,R
            //  but we no longer rely on this elsewhere as it
            //  stops us reordering moves
            moves.push_back({square, dst, SPECIAL_PROMOTION_QUEEN,  ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_KNIGHT, ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_BISHOP, ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_ROOK,   ' '});
        }
    }
}

// Generate list of black pawn moves
void ChessRules::BlackPawnMoves(vector<Move>& moves, Square square) {
    const lte* ptr = pawn_black_lookup[square];
    bool promotion = RANK(square) == '2';

    // Capture ray
    lte nbr_moves = *ptr++;
    while (--nbr_moves >= 0) {
        const Square dst = static_cast<Square>(*ptr++);
        if (dst == enpassant_target) {
            moves.push_back({square, dst, SPECIAL_BEN_PASSANT, 'P'});
        }
        else if (IsWhite(squares[dst])) {
            const int capture = squares[dst];
            if (!promotion) {
                moves.push_back({square, dst, NOT_SPECIAL, capture});
            }
            else {
                // Generate (under)promotions in the order (Q),N,B,R
                //  but we no longer rely on this elsewhere as it
                //  stops us reordering moves
                moves.push_back({square, dst, SPECIAL_PROMOTION_QUEEN,  capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_KNIGHT, capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_BISHOP, capture});
                moves.push_back({square, dst, SPECIAL_PROMOTION_ROOK,   capture});
            }
        }
    }

    // Advance ray
    nbr_moves = *ptr++;
    for (int i = 0; i < nbr_moves; ++i) {
        const Square dst = static_cast<Square>(*ptr++);

        // If square occupied, end now
        if (!IsEmptySquare(squares[dst])) {
            break;
        }
        if (!promotion) {
            moves.push_back({square, dst, i == 0 ? NOT_SPECIAL : SPECIAL_BPAWN_2SQUARES, ' '});
        }
        else {
            // Generate (under)promotions in the order (Q),N,B,R
            //  but we no longer rely on this elsewhere as it
            //  stops us reordering moves
            moves.push_back({square, dst, SPECIAL_PROMOTION_QUEEN,  ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_KNIGHT, ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_BISHOP, ' '});
            moves.push_back({square, dst, SPECIAL_PROMOTION_ROOK,   ' '});
        }
    }
}

/****************************************************************************
 * Make a move (with the potential to undo)
 ****************************************************************************/
void ChessRules::PushMove(Move& m) {
    // Push old details onto stack
    DETAIL_PUSH;

    // Update castling prohibited flags for destination square, eg h8 -> bking
    DETAIL_CASTLING(m.dst);
                    // IMPORTANT - only dst is required since we also qualify
                    //  castling with presence of rook and king on right squares.
                    //  (I.E. if a rook or king leaves its original square, the
                    //  castling prohibited flag is unaffected, but it doesn't
                    //  matter since we won't castle unless rook and king are
                    //  present on the right squares. If subsequently a king or
                    //  rook returns, that's okay too because the  castling flag
                    //  is cleared by its arrival on the m.dst square, so
                    //  castling remains prohibited).
    enpassant_target = SQUARE_INVALID;

    // Special handling might be required
    switch (m.special) {
    default:
        squares[m.dst] = squares[m.src];
        squares[m.src] = ' ';
        break;

    // King move updates king position in details field
    case SPECIAL_KING_MOVE:
        squares[m.dst] = squares[m.src];
        squares[m.src] = ' ';
        if( white )
            wking_square = m.dst;
        else
            bking_square = m.dst;
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_QUEEN:
        squares[m.src] = ' ';
        squares[m.dst] = (white?'Q':'q');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_ROOK:
        squares[m.src] = ' ';
        squares[m.dst] = (white?'R':'r');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_BISHOP:
        squares[m.src] = ' ';
        squares[m.dst] = (white?'B':'b');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_KNIGHT:
        squares[m.src] = ' ';
        squares[m.dst] = (white?'N':'n');
        break;

    // White enpassant removes pawn south of destination
    case SPECIAL_WEN_PASSANT:
        squares[m.src] = ' ';
        squares[m.dst] = 'P';
        squares[ SOUTH(m.dst) ] = ' ';
        break;

    // Black enpassant removes pawn north of destination
    case SPECIAL_BEN_PASSANT:
        squares[m.src] = ' ';
        squares[m.dst] = 'p';
        squares[ NORTH(m.dst) ] = ' ';
        break;

    // White pawn advances 2 squares sets an enpassant target
    case SPECIAL_WPAWN_2SQUARES:
        squares[m.src] = ' ';
        squares[m.dst] = 'P';
        enpassant_target = SOUTH(m.dst);
        break;

    // Black pawn advances 2 squares sets an enpassant target
    case SPECIAL_BPAWN_2SQUARES:
        squares[m.src] = ' ';
        squares[m.dst] = 'p';
        enpassant_target = NORTH(m.dst);
        break;

    // Castling moves update 4 squares each
    case SPECIAL_WK_CASTLING:
        squares[e1] = ' ';
        squares[f1] = 'R';
        squares[g1] = 'K';
        squares[h1] = ' ';
        wking_square = g1;
        break;
    case SPECIAL_WQ_CASTLING:
        squares[e1] = ' ';
        squares[d1] = 'R';
        squares[c1] = 'K';
        squares[a1] = ' ';
        wking_square = c1;
        break;
    case SPECIAL_BK_CASTLING:
        squares[e8] = ' ';
        squares[f8] = 'r';
        squares[g8] = 'k';
        squares[h8] = ' ';
        bking_square = g8;
        break;
    case SPECIAL_BQ_CASTLING:
        squares[e8] = ' ';
        squares[d8] = 'r';
        squares[c8] = 'k';
        squares[a8] = ' ';
        bking_square = c8;
        break;
    }

    // Toggle who-to-move
    Toggle();
}

/****************************************************************************
 * Undo a move
 ****************************************************************************/
void ChessRules::PopMove(Move& m) {
    // Previous detail field
    DETAIL_POP;

    // Toggle who-to-move
    Toggle();

    // Special handling might be required
    switch (m.special) {
    default:
        squares[m.src] = squares[m.dst];
        squares[m.dst] = m.capture;
        break;

    // For promotion, src piece was a pawn
    case SPECIAL_PROMOTION_QUEEN:
    case SPECIAL_PROMOTION_ROOK:
    case SPECIAL_PROMOTION_BISHOP:
    case SPECIAL_PROMOTION_KNIGHT:
        if( white )
            squares[m.src] = 'P';
        else
            squares[m.src] = 'p';
        squares[m.dst] = m.capture;
        break;

    // White enpassant re-insert black pawn south of destination
    case SPECIAL_WEN_PASSANT:
        squares[m.src] = 'P';
        squares[m.dst] = ' ';
        squares[SOUTH(m.dst)] = 'p';
        break;

    // Black enpassant re-insert white pawn north of destination
    case SPECIAL_BEN_PASSANT:
        squares[m.src] = 'p';
        squares[m.dst] = ' ';
        squares[NORTH(m.dst)] = 'P';
        break;

    // Castling moves update 4 squares each
    case SPECIAL_WK_CASTLING:
        squares[e1] = 'K';
        squares[f1] = ' ';
        squares[g1] = ' ';
        squares[h1] = 'R';
        break;
    case SPECIAL_WQ_CASTLING:
        squares[e1] = 'K';
        squares[d1] = ' ';
        squares[c1] = ' ';
        squares[a1] = 'R';
        break;
    case SPECIAL_BK_CASTLING:
        squares[e8] = 'k';
        squares[f8] = ' ';
        squares[g8] = ' ';
        squares[h8] = 'r';
        break;
    case SPECIAL_BQ_CASTLING:
        squares[e8] = 'k';
        squares[d8] = ' ';
        squares[c8] = ' ';
        squares[a8] = 'r';
        break;
    }
}

/****************************************************************************
 * Determine if an occupied square is attacked
 ****************************************************************************/
bool ChessRules::AttackedPiece(Square square) {
    const bool enemy_is_white = IsBlack(squares[square]);
    return AttackedSquare(square, enemy_is_white);
}

/****************************************************************************
 * Is a square is attacked by enemy ?
 ****************************************************************************/
bool ChessRules::AttackedSquare(Square square, bool enemy_is_white) {
    Square dst;
    const lte *ptr = (enemy_is_white ? attacks_black_lookup[square] : attacks_white_lookup[square] );
    lte nbr_rays = *ptr++;
    while( nbr_rays-- ) {
        lte ray_len = *ptr++;
        while( ray_len-- ) {
            dst = (Square)*ptr++;
            char piece=squares[dst];

            // If square not occupied (empty), continue
            if( IsEmptySquare(piece) )
                ptr++;  // skip mask

            // Else if occupied
            else {
                lte mask = *ptr++;

                // White attacker ?
                if( IsWhite(piece) && enemy_is_white ) {
                    if( to_mask[piece] & mask )
                        return true;
                }

                // Black attacker ?
                else if( IsBlack(piece) && !enemy_is_white ) {
                    if( to_mask[piece] & mask )
                        return true;
                }

                // Goto end of ray
                ptr += (2*ray_len);
                ray_len = 0;
            }
        }
    }

    ptr = knight_lookup[square];
    lte nbr_squares = *ptr++;
    while( nbr_squares-- ) {
        dst = (Square)*ptr++;
        char piece=squares[dst];

        // If occupied by an enemy knight, we have found an attacker
        if( (enemy_is_white&&piece=='N') || (!enemy_is_white&&piece=='n') )
            return true;
    }
    return false;
}

/****************************************************************************
 * Evaluate a position, returns bool okay (not okay means illegal position)
 ****************************************************************************/
bool ChessRules::Evaluate() {
    Square enemy_king = (Square)(white ? bking_square : wking_square);
    // Enemy king is attacked and our move, position is illegal
    return !AttackedPiece(enemy_king);
}

bool ChessRules::Evaluate(TERMINAL &score_terminal) {
    return Evaluate(nullptr, score_terminal);
}

bool ChessRules::Evaluate(vector<Move> *p, TERMINAL& score_terminal) {
    vector<Move> local_list;
    vector<Move>& list = p ? *p : local_list;
    int i, any;
    Square my_king, enemy_king;
    bool okay;
    score_terminal=NOT_TERMINAL;

    // Enemy king is attacked and our move, position is illegal
    enemy_king = (Square)(white ? bking_square : wking_square);
    if( AttackedPiece(enemy_king) )
        okay = false;

    // Else legal position
    else {
        okay = true;

        // Work out if the game is over by checking for any legal moves
        GenMoveList(list);
        any = 0;
        for (auto move: list) {
            PushMove(move);
            my_king = (Square)(white ? bking_square : wking_square);
            if( !AttackedPiece(my_king) )
                any++;
            PopMove(move);
        }

        // If no legal moves, position is either checkmate or stalemate
        if( any == 0 ) {
            my_king = (Square)(white ? wking_square : bking_square);
            if( AttackedPiece(my_king) )
                score_terminal = (white ? TERMINAL_WCHECKMATE
                                        : TERMINAL_BCHECKMATE);
            else
                score_terminal = (white ? TERMINAL_WSTALEMATE
                                        : TERMINAL_BSTALEMATE);
        }
    }
    return okay;
}

/****************************************************************************
 *  Test for legal position, sets reason to a mask of possibly multiple reasons
 ****************************************************************************/
bool ChessRules::IsLegal(ILLEGAL_REASON& reason) {
    int  ireason = 0;
    int  wkings=0, bkings=0, wpawns=0, bpawns=0, wpieces=0, bpieces=0;
    bool legal = true;
    int  file, rank;
    char p;
    Square opposition_king_location = SQUARE_INVALID;

    // Loop through the board
    file=0;     // go from a8,b8..h8,a7,b7..h1
    rank=7;
    for (;;) {
        Square sq = SQ('a'+file, '1'+rank);
        p = squares[sq];
        if ((p == 'P' || p == 'p') && (rank == 0 || rank == 7)) {
            legal = false;
            ireason |= IR_PAWN_POSITION;
        }
        if (IsWhite(p)) {
            if (p == 'P') {
                wpawns++;
            }
            else {
                wpieces++;
                if (p == 'K') {
                    wkings++;
                    if (!white) {
                        opposition_king_location = sq;
                    }
                }
            }
        }
        else if (IsBlack(p)) {
            if (p == 'p') {
                bpawns++;
            }
            else {
                bpieces++;
                if (p == 'k') {
                    bkings++;
                    if (white) {
                        opposition_king_location = sq;
                    }
                }
            }
        }
        if( sq == h1 ) {
            break;
        }
        else {
            file++;
            if (file == 8) {
                file = 0;
                rank--;
            }
        }
    }
    if (wkings!=1 || bkings!=1) {
        legal = false;
        ireason |= IR_NOT_ONE_KING_EACH;
    }
    if (opposition_king_location!=SQUARE_INVALID && AttackedPiece(opposition_king_location)) {
        legal = false;
        ireason |= IR_CAN_TAKE_KING;
    }
    if (wpieces>8 && (wpieces+wpawns)>16) {
        legal = false;
        ireason |= IR_WHITE_TOO_MANY_PIECES;
    }
    if (bpieces>8 && (bpieces+bpawns)>16) {
        legal = false;
        ireason |= IR_BLACK_TOO_MANY_PIECES;
    }
    if (wpawns > 8) {
        legal = false;
        ireason |= IR_WHITE_TOO_MANY_PAWNS;
    }
    if (bpawns > 8) {
        legal = false;
        ireason |= IR_BLACK_TOO_MANY_PAWNS;
    }
    reason = static_cast<ILLEGAL_REASON>(ireason);
    return legal;
}
