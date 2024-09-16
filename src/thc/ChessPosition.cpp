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
    int   file, rank, skip, store, temp;
    int   count_wking=0, count_bking=0;
    char  c, cross;
    char  p;
    bool okay, done;
    const char *reset = txt;

    // When store==0 validate txt without storing results
    for( store=0, okay=true; store<2 && okay; store++ )
    {
        txt = reset;

        // Clear the board
        if( store )
        {
            for( Square square=a8; square<=h1; ++square )
            {
                squares[square]   = ' ';
            }

            // Clear the extension fields
            d.wking(false);
            d.wqueen(false);
            d.bking(false);
            d.bqueen(false);
            d.enpassant_target = SQUARE_INVALID;
            half_move_clock = 0;
            full_move_count = 1;
        }

        // Loop through the main Forsyth field
        for( file=0, rank=7, done=false; *txt && okay && !done; )
        {
            skip = 1;
            c = *txt++;
            p = ' ';
            cross = ' ';
            switch(c)
            {
                case 'x':   cross = 'x';
                            skip = 1;
                            break;
                case ' ':
                case '\t':  done = true;            break;
                case 'k':   p = 'k';
                            count_bking++;          break;
                case 'K':   p = 'K';
                            count_wking++;          break;
                case 'p':
                case 'r':
                case 'n':
                case 'b':
                case 'q':
                case 'P':
                case 'R':
                case 'N':
                case 'B':
                case 'Q':   p = c;   break;
                case '1':   case '2':   case '3':   case '4':
                case '5':   case '6':   case '7':   case '8':
                            skip = c-'0';       break;
                case '/':   // official separator
                case '|':   //  .. we'll allow this
                case '\\':  //  .. or this
                {
                    if( file == 0 )
                        skip = 0;        // eg after "rn6/", '/' does nothing
                    else
                        skip = (8-file); // eg after "rn/", '/' skips 6 squares
                    while( *txt == '/'   // allow consecutive '/' characters
                      ||   *txt == '|'   //     (or '|' or '\')
                      ||   *txt == '\\' )
                    {
                        txt++;
                        skip += 8;
                    }
                    break;
                }
                default:    okay=false;
            }

            // Store single piece or loop over multiple empty squares
            for( int i=0; i<skip && okay && !done; i++ )
            {
                Square sq = SQ('a'+file, '1'+rank);
                if( store )
                {
                    squares[sq] = p;
                    if( p == 'K' )
                        d.wking_square = sq;
                    else if( p == 'k' )
                        d.bking_square = sq;
                }
                file++;
                if( file == 8 )
                {
                    file = 0;
                    rank--;
                }
                if( sq == h1 )
                    done = true;        // last square, we're done !
            }
        }

        // In validation pass make sure there's 1 white king and 1 black king
   /*   if( store==0 )  // disabled to allow ILLEGAL_NOT_ONE_KING_EACH message
        {
            if( count_wking!=1 || count_bking!=1 )
                okay = false;
        }  */

        // Now support standard FEN notation with extensions. See appendix to
        // .PGN standard on the Internet (and in project documentation)
        //
        // Briefly there are a total of 6 fields, the normal Forsyth encoding
        // (as above) plus 5 extension fields. Format of the 5 extension
        // fields is illustrated by example.
        //
        // Example extension fields; "w KQq e6 0 2"
        //
        // White to move,
        // All types of castling available, except black king side (so black's
        //  king rook has moved, other king and rook pieces unmoved). This
        //  whole field is replaced by '-' character if no castling available.
        // The last move played was a double pawn advance, and hence the
        //  'en-passant target square' field is NOT empty. The field is the
        //  square a potential capturing pawn would end up on (can deduce last
        //  move was e7e5)
        // Number of half-moves or ply since a pawn move or capture (for 50
        //  move rule)
        // Total (full-move) count (starts at 1, increments after black moves)

        // Who to move
        if( okay )
        {
            if( *txt == '/'
             || *txt == '|'
             || *txt == '\\' )
                txt++;
            while( *txt==' ' || *txt=='\t' )
                txt++;
            if( *txt=='W' || *txt=='w' )
            {
                if( store )
                    white = true;
                txt++;
            }
            else if( *txt=='B' || *txt=='b' )
            {
                if( store )
                    white = false;
                txt++;
            }
            else
                okay = false;
        }

        // Castling flags
        if( okay )
        {
            while( *txt==' ' || *txt=='\t' )
                txt++;
            if( *txt == '-' )
                txt++;
            else
            {
                for( int i=0; i<4 && okay; i++, txt++ )
                {
                    if( *txt == 'K' )
                    {
                        if( store )
                            d.wking(true);
                    }
                    else if( *txt == 'Q' )
                    {
                        if( store )
                            d.wqueen(true);
                    }
                    else if( *txt == 'k' )
                    {
                        if( store )
                            d.bking(true);
                    }
                    else if( *txt == 'q' )
                    {
                        if( store )
                            d.bqueen(true);
                    }
                    else if( *txt == '-' )               // allow say "KQ-q "
                        ;
                    else if( *txt == ' ' || *txt=='\t' ) // or "KQq "
                        break;
                    else
                        okay = false;
                }
            }
        }

        // Enpassant target
        if( okay )
        {
            while( *txt==' ' || *txt=='\t' )
                txt++;
            if( *txt == '-' )
                txt++;
            else
            {
                char f='a', r='1';
                if( 'a'<=*txt && *txt<='h' )
                    f = *txt++;
                else if( 'A'<=*txt && *txt<='H' )
                {
                    f = *txt++;
                    f = f-'A'+'a';
                }
                else
                    okay = false;
                if( okay )
                {
                    if( '1'<=*txt && *txt<='8' )
                        r = *txt++;
                    else
                        okay = false;
                }
                if( okay && store )
                    d.enpassant_target = SQ(f,r);
            }
        }

        // Half move clock
        if( okay )
        {
            okay=false;
            while( *txt==' ' || *txt=='\t' )
                txt++;
            temp = atoi(txt);
            if( temp >= 0 )
            {
                while( isascii(*txt) && isdigit(*txt) )
                {
                    okay = true;
                    txt++;
                }
                if( okay && store )
                    half_move_clock = temp;
            }
        }

        // Full move count
        if( okay )
        {
            while( *txt==' ' || *txt=='\t' )
                txt++;
            temp = atoi(txt);
            if( temp < 0 )
                okay = false;
            else if( store )
                full_move_count = temp;
        }
    }
    return( okay );
}


// Publish chess position and supplementary info in forsyth notation
std::string ChessPosition::fen() const {
    int i, empty=0, file=0, rank=7, save_file=0, save_rank=0;
    Square sq;
    char p;
    std::string str;

    // Squares
    for( i=0; i<64; i++ )
    {
        sq = SQ('a'+file,'1'+rank);
        if( sq == d.enpassant_target )
        {
            save_file = file;
            save_rank = rank;
        }
        p = squares[sq];
        if( p == ' ' )
            empty++;
        else
        {
            if( empty )
            {
                char count = '0' + (char)empty;
                str += count;
                empty = 0;
            }
            str += p;
        }
        file++;
        if( file == 8 )
        {
            if( empty )
            {
                char count = '0'+(char)empty;
                str += count;
            }
            if( rank )
                str += '/';
            empty = 0;
            file = 0;
            rank--;
        }
    }

    // Who to move
    str += ' ';
    str += (white?'w':'b');

    // Castling flags
    str += ' ';
    if( !wking_allowed() && !wqueen_allowed() && !bking_allowed() && !bqueen_allowed() )
        str += '-';
    else
    {
        if( wking_allowed() )
            str += 'K';
        if( wqueen_allowed() )
            str += 'Q';
        if( bking_allowed() )
            str += 'k';
        if( bqueen_allowed() )
            str += 'q';
    }

    // Enpassant target square
    str += ' ';
    if( d.enpassant_target==SQUARE_INVALID || save_rank==0 )
        str += '-';
    else
    {
        char file2 = 'a'+(char)save_file;
        str += file2;
        char rank2 = '1'+(char)save_rank;
        str += rank2;
    }

    // Counts
    char buf[40];
    sprintf( buf, " %d %d", half_move_clock, full_move_count );
    str += buf;
    return( str );
}


Move ChessPosition::uci_move(string_view uci_move) const {
    const auto expected = Move(uci_move);
    for (const auto move : legal_moves()) {
        if (move.src != expected.src || move.dst != expected.dst) {
            continue;
        }
        if (move.is_promotion() || expected.is_promotion()) {
            if (move.special != expected.special) {
                continue;
            }
        }
        return move;
    }
    throw domain_error("Invalid UCI move: " + string(uci_move));
}


string ChessPosition::move_uci(const Move& move) const {
    return move.uci();
}


// Read natural string move eg "Nf3"
Move ChessPosition::san_move(string_view natural_in) const {
    int  i, len=0;
    char src_file='\0', src_rank='\0', dst_file='\0', dst_rank='\0';
    char promotion='\0';
    bool enpassant=false;
    bool kcastling=false;
    bool qcastling=false;
    Square dst_=a8;
    Move *m, *found=NULL;
    char *s;
    char  move[10];
    bool  white=this->white;
    char  piece=(white?'P':'p');
    bool  default_piece=true;

    // Indicate no move found (yet)
    bool okay=true;

    // Copy to read-write variable
    okay = false;
    for( i=0; i<sizeof(move); i++ )
    {
        move[i] = natural_in[i];
        if( move[i]=='\0' || move[i]==' ' || move[i]=='\t' ||
            move[i]=='\r' || move[i]=='\n' )
        {
            move[i] = '\0';
            okay = true;
            break;
        }
    }
    if( okay )
    {

        // Trim string from end
        s = strchr(move,'\0') - 1;
        while( s>=move && !(isascii(*s) && isalnum(*s)) )
            *s-- = '\0';

        // Trim string from start
        s = move;
        while( *s==' ' || *s=='\t' )
            s++;
        len = (int)strlen(s);
        for( i=0; i<len+1; i++ )  // +1 gets '\0' at end
            move[i] = *s++;  // if no leading space this does
                            //  nothing, but no harm either

        // Trim enpassant
        if( len>=2 && move[len-1]=='p' )
        {
            if( 0 == strcmp(&move[len-2],"ep") )
            {
                move[len-2] = '\0';
                enpassant = true;
            }
            else if( len>=3 && 0==strcmp(&move[len-3],"e.p") )
            {
                move[len-3] = '\0';
                enpassant = true;
            }

            // Trim string from end, again
            s = strchr(move,'\0') - 1;
            while( s>=move && !(isascii(*s) && isalnum(*s)) )
                *s-- = '\0';
            len = (int)strlen(move);
        }

        // Promotion
        if( len>2 )  // We are supporting "ab" to mean Pawn a5xb6 (say), and this test makes sure we don't
        {            // mix that up with a lower case bishop promotion, and that we don't reject "ef" say
                     // on the basis that 'F' is not a promotion indication. We've never supported "abQ" say
                     // as a7xb8=Q, and we still don't so "abb" as a bishop promotion doesn't work, but we
                     // continue to support "ab=Q", and even "ab=b".
                     // The test also ensures we can access move[len-2] below
                     // These comments added when we changed the logic to support "b8Q" and "a7xb8Q", the
                     // '=' can optionally be omitted in such cases, the first change in this code for many,
                     // many years.
            char last = move[len-1];
            bool is_file = ('1'<=last && last<='8');
            if( !is_file )
            {
                switch( last )
                {
                    case 'O':
                    case 'o':   break;  // Allow castling!
                    case 'q':
                    case 'Q':   promotion='Q';  break;
                    case 'r':
                    case 'R':   promotion='R';  break;
                    case 'b':   if( len==3 && '2'<=move[1] && move[1]<='7' )
                                    break;  // else fall through to promotion - allows say "a5b" as disambiguating
                                            //  version of "ab" if there's more than one "ab" available! Something
                                            //  of an ultra refinement
                    case 'B':   promotion='B';  break;
                    case 'n':
                    case 'N':   promotion='N';  break;
                    default:    okay = false;   break;   // Castling and promotions are the only cases longer than 2
                                                         //  chars where a non-file ends a move. (Note we still accept
                                                         //  2 character pawn captures like "ef").
                }
                if( promotion )
                {
                    switch( move[len-2] )
                    {
                        case '=':
                        case '1':   // we now allow '=' to be omitted, as e.g. ChessBase mobile seems to (sometimes?)
                        case '8':   break;
                        default:    okay = false;   break;
                    }
                    if( okay )
                    {

                        // Trim string from end, again
                        move[len-1] = '\0';     // Get rid of 'Q', 'N' etc
                        s = move + len-2;
                        while( s>=move && !(isascii(*s) && isalnum(*s)) )
                            *s-- = '\0';    // get rid of '=' but not '1','8'
                        len = (int)strlen(move);
                    }
                }
            }
        }
    }

    // Castling
    if( okay )
    {
        if( 0==strcmp_ignore(move,"oo") || 0==strcmp_ignore(move,"o-o") )
        {
            strcpy( move, (white?"e1g1":"e8g8") );
            len       = 4;
            piece     = (white?'K':'k');
            default_piece = false;
            kcastling = true;
        }
        else if( 0==strcmp_ignore(move,"ooo") || 0==strcmp_ignore(move,"o-o-o") )
        {
            strcpy( move, (white?"e1c1":"e8c8") );
            len       = 4;
            piece     = (white?'K':'k');
            default_piece = false;
            qcastling = true;
        }
    }

    // Destination square for all except pawn takes pawn (eg "ef")
    if( okay )
    {
        if( len==2 && 'a'<=move[0] && move[0]<='h'
                   && 'a'<=move[1] && move[1]<='h' )
        {
            src_file = move[0]; // eg "ab" pawn takes pawn
            dst_file = move[1];
        }
        else if( len==3 && 'a'<=move[0] && move[0]<='h'
                        && '2'<=move[1] && move[1]<='7'
                        && 'a'<=move[2] && move[2]<='h' )
        {
            src_file = move[0]; // eg "a3b"  pawn takes pawn
            dst_file = move[2];
        }
        else if( len>=2 && 'a'<=move[len-2] && move[len-2]<='h'
                        && '1'<=move[len-1] && move[len-1]<='8' )
        {
            dst_file = move[len-2];
            dst_rank = move[len-1];
            dst_ = SQ(dst_file,dst_rank);
        }
        else
            okay = false;
    }

    // Source square and or piece
    if( okay )
    {
        if( len > 2 )
        {
            if( 'a'<=move[0] && move[0]<='h' &&
                '1'<=move[1] && move[1]<='8' )
            {
                src_file = move[0];
                src_rank = move[1];
            }
            else
            {
                switch( move[0] )
                {
                    case 'K':   piece = (white?'K':'k');    default_piece=false; break;
                    case 'Q':   piece = (white?'Q':'q');    default_piece=false; break;
                    case 'R':   piece = (white?'R':'r');    default_piece=false; break;
                    case 'N':   piece = (white?'N':'n');    default_piece=false; break;
                    case 'P':   piece = (white?'P':'p');    default_piece=false; break;
                    case 'B':   piece = (white?'B':'b');    default_piece=false; break;
                    default:
                    {
                        if( 'a'<=move[0] && move[0]<='h' )
                            src_file = move[0]; // eg "ef4"
                        else
                            okay = false;
                        break;
                    }
                }
                if( len>3  && src_file=='\0' )  // not eg "ef4" above
                {
                    if( '1'<=move[1] && move[1]<='8' )
                        src_rank = move[1];
                    else if( 'a'<=move[1] && move[1]<='h' )
                    {
                        src_file = move[1];
                        if( len>4 && '1'<=move[2] && move[2]<='8' )
                            src_rank = move[2];
                    }
                }
            }
        }
    }

    // Check against all possible moves
    if( okay )
    {
        auto list = legal_moves();

        // Have source and destination, eg "d2d3"
        if( enpassant )
            src_rank = dst_rank = '\0';
        if( src_file && src_rank && dst_file && dst_rank )
        {
            for (auto& m : list) {
                if( (default_piece || piece==squares[m.src])  &&
                    src_file  ==   FILE(m.src)       &&
                    src_rank  ==   RANK(m.src)       &&
                    dst_       ==   m.dst
                )
                {
                    if( kcastling )
                    {
                        if( m.special ==
                             (white?SPECIAL_WK_CASTLING:SPECIAL_BK_CASTLING) )
                            found = &m;
                    }
                    else if( qcastling )
                    {
                        if( m.special ==
                             (white?SPECIAL_WQ_CASTLING:SPECIAL_BQ_CASTLING) )
                            found = &m;
                    }
                    else
                        found = &m;
                    break;
                }
            }
        }

        // Have source file only, eg "Rae1"
        else if( src_file && dst_file && dst_rank )
        {
            for (auto& m : list) {
                if( piece     ==   squares[m.src]  &&
                    src_file  ==   FILE(m.src)         &&
                 /* src_rank  ==   RANK(m.src)  */
                    dst_       ==   m.dst
                )
                {
                    found = &m;
                    break;
                }
            }
        }

        // Have source rank only, eg "R2d2"
        else if( src_rank && dst_file && dst_rank )
        {
            for (auto& m : list) {
                if( piece     ==   squares[m.src]   &&
                 /* src_file  ==   FILE(m.src) */
                    src_rank  ==   RANK(m.src)          &&
                    dst_       ==   m.dst
                )
                {
                    found = &m;
                    break;
                }
            }
        }

        // Have destination file only eg e4f (because 2 ef moves are possible)
        else if( src_file && src_rank && dst_file )
        {
            for (auto& m : list) {
                if( piece     ==   squares[m.src]      &&
                    src_file  ==   FILE(m.src)             &&
                    src_rank  ==   RANK(m.src)             &&
                    dst_file  ==   FILE(m.dst)
                )
                {
                    found = &m;
                    break;
                }
            }
        }

        // Have files only, eg "ef"
        else if( src_file && dst_file )
        {
            for (auto& m : list) {
                if( piece     ==   squares[m.src]      &&
                    src_file  ==   FILE(m.src)             &&
                 /* src_rank  ==   RANK(m.src) */
                    dst_file  ==   FILE(m.dst)
                )
                {
                    if( enpassant )
                    {
                        if( m.special ==
                             (white?SPECIAL_WEN_PASSANT:SPECIAL_BEN_PASSANT) )
                            found = &m;
                    }
                    else
                        found = &m;
                    break;
                }
            }
        }

        // Have destination square only eg "a4"
        else if( dst_rank && dst_file )
        {
            for (auto& m : list) {
                if( piece     ==   squares[m.src]          &&
                    dst_       ==   m.dst
                )
                {
                    found = &m;
                    break;
                }
            }
        }
    }

    // Copy found move
    if( okay && found )
    {
        bool found_promotion =
            ( found->special == SPECIAL_PROMOTION_QUEEN ||
              found->special == SPECIAL_PROMOTION_ROOK ||
              found->special == SPECIAL_PROMOTION_BISHOP ||
              found->special == SPECIAL_PROMOTION_KNIGHT
            );
        if( promotion && !found_promotion )
            okay = false;
        if( found_promotion )
        {
            switch( promotion )
            {
                default:
                case 'Q': found->special = SPECIAL_PROMOTION_QUEEN;   break;
                case 'R': found->special = SPECIAL_PROMOTION_ROOK;    break;
                case 'B': found->special = SPECIAL_PROMOTION_BISHOP;  break;
                case 'N': found->special = SPECIAL_PROMOTION_KNIGHT;  break;
            }
        }
    }

    if (!okay || !found) {
        throw std::domain_error("Invalid SAN move: " + string(natural_in));
    }
    return *found;
}


string ChessPosition::move_san(const Move& move) const {
// Improved algorithm

    /* basic procedure is run the following algorithms in turn:
        pawn move     ?
        castling      ?
        Nd2 or Nxd2   ? (loop through all legal moves check if unique)
        Nbd2 or Nbxd2 ? (loop through all legal moves check if unique)
        N1d2 or N1xd2 ? (loop through all legal moves check if unique)
        Nb1d2 or Nb1xd2 (fallback if nothing else works)
    */

    char nmove[10];
    nmove[0] = '-';
    nmove[1] = '-';
    nmove[2] = '\0';
    vector<Move> list;
    vector<bool> check;
    vector<bool> mate;
    vector<bool> stalemate;
    enum
    {
        ALG_PAWN_MOVE,
        ALG_CASTLING,
        ALG_ND2,
        ALG_NBD2,
        ALG_N1D2,
        ALG_NB1D2
    };
    bool done=false;
    bool found = false;
    char append='\0';
    thc::GenLegalMoveList(*this, list, check, mate, stalemate);
    for (int i = 0; i != list.size(); ++i) {
        Move mfound = list[i];
        if( mfound == move )
        {
            found = true;
            if( mate[i] )
                append = '#';
            else if( check[i] )
                append = '+';
        }
    }

    // Loop through algorithms
    for( int alg=ALG_PAWN_MOVE; found && !done && alg<=ALG_NB1D2; alg++ )
    {
        bool do_loop = (alg==ALG_ND2 || alg==ALG_NBD2 || alg==ALG_N1D2);
        int matches=0;

        // Run the algorithm on the input move (i=-1) AND on all legal moves
        //  in a loop if do_loop set for this algorithm (i=0 to i=count-1)
        for (auto i = -1; !done && i < (do_loop ? int(list.size()) : 0); i++) {
            char compare[10];
            char *str_dst = (i == -1) ? nmove : compare;
            Move m = (i == -1) ? move : list[i];
            Square src_ = m.src;
            Square dst_ = m.dst;
            char t, p = squares[src_];
            if( islower(p) )
                p = (char)toupper(p);
            if( !IsEmptySquare(m.capture) ) // until we did it this way, enpassant was '-' instead of 'x'
                t = 'x';
            else
                t = '-';
            switch( alg )
            {
                // pawn move ? "e4" or "exf6", plus "=Q" etc if promotion
                case ALG_PAWN_MOVE:
                {
                    if( p == 'P' )
                    {
                        done = true;
                        if( t == 'x' )
                            sprintf( nmove, "%cx%c%c", FILE(src_),FILE(dst_),RANK(dst_) );
                        else
                            sprintf( nmove, "%c%c",FILE(dst_),RANK(dst_) );
                        char *s = strchr(nmove,'\0');
                        switch( m.special )
                        {
                            case SPECIAL_PROMOTION_QUEEN:
                                strcpy( s, "=Q" );  break;
                            case SPECIAL_PROMOTION_ROOK:
                                strcpy( s, "=R" );  break;
                            case SPECIAL_PROMOTION_BISHOP:
                                strcpy( s, "=B" );  break;
                            case SPECIAL_PROMOTION_KNIGHT:
                                strcpy( s, "=N" );  break;
                            default:
                                break;
                        }
                    }
                    break;
                }

                // castling ?
                case ALG_CASTLING:
                {
                    if( m.special==SPECIAL_WK_CASTLING || m.special==SPECIAL_BK_CASTLING )
                    {
                        strcpy( nmove, "O-O" );
                        done = true;
                    }
                    else if( m.special==SPECIAL_WQ_CASTLING || m.special==SPECIAL_BQ_CASTLING )
                    {
                        strcpy( nmove, "O-O-O" );
                        done = true;
                    }
                    break;
                }

                // Nd2 or Nxd2
                case ALG_ND2:
                {
                    if( t == 'x' )
                        sprintf( str_dst, "%cx%c%c", p, FILE(dst_), RANK(dst_) );
                    else
                        sprintf( str_dst, "%c%c%c", p, FILE(dst_), RANK(dst_) );
                    if( i >= 0 )
                    {
                        if( 0 == strcmp(nmove,compare) )
                            matches++;
                    }
                    break;
                }

                // Nbd2 or Nbxd2
                case ALG_NBD2:
                {
                    if( t == 'x' )
                        sprintf( str_dst, "%c%cx%c%c", p, FILE(src_), FILE(dst_), RANK(dst_) );
                    else
                        sprintf( str_dst, "%c%c%c%c", p, FILE(src_), FILE(dst_), RANK(dst_) );
                    if( i >= 0 )
                    {
                        if( 0 == strcmp(nmove,compare) )
                            matches++;
                    }
                    break;
                }

                // N1d2 or N1xd2
                case ALG_N1D2:
                {
                    if( t == 'x' )
                        sprintf( str_dst, "%c%cx%c%c", p, RANK(src_), FILE(dst_), RANK(dst_) );
                    else
                        sprintf( str_dst, "%c%c%c%c", p, RANK(src_), FILE(dst_), RANK(dst_) );
                    if( i >= 0 )
                    {
                        if( 0 == strcmp(nmove,compare) )
                            matches++;
                    }
                    break;
                }

                //  Nb1d2 or Nb1xd2
                case ALG_NB1D2:
                {
                    done = true;
                    if( t == 'x' )
                        sprintf( nmove, "%c%c%cx%c%c", p, FILE(src_), RANK(src_), FILE(dst_), RANK(dst_) );
                    else
                        sprintf( nmove, "%c%c%c%c%c", p, FILE(src_), RANK(src_), FILE(dst_), RANK(dst_) );
                    break;
                }
            }
        }   // end loop for all legal moves with given algorithm

        // If it's a looping algorithm and only one move matches nmove, we're done
        if( do_loop && matches==1 )
            done = true;
    }   // end loop for all algorithms
    if( append )
    {
        char *s = strchr(nmove,'\0');
        *s++ = append;
        *s = '\0';
    }
    return nmove;
}
