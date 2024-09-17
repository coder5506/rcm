/****************************************************************************
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "san.h"
#include "ChessPosition.h"
#include "gen.h"

#include <cstring>
#include <stdexcept>

using namespace std;
using namespace thc;


string san::from_move(const ChessPosition& position, const Move& move) {
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
    gen::GenLegalMoveList(position, list, check, mate, stalemate);
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
            char t, p = position.squares[src_];
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


Move san::to_move(const ChessPosition& position, string_view san_move) {
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
    bool  white=position.white;
    char  piece=(white?'P':'p');
    bool  default_piece=true;

    // Indicate no move found (yet)
    auto okay = false;

    // Copy to read-write variable
    for( i=0; i<sizeof(move); i++ )
    {
        move[i] = san_move[i];
        if( move[i]=='\0' || move[i]==' ' || move[i]=='\t' ||
            move[i]=='\r' || move[i]=='\n' )
        {
            move[i] = '\0';
            okay = true;
            break;
        }
    }
    if (!okay) {
        throw domain_error("Invalid SAN move: " + string(san_move));
    }

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
                default:
                    throw domain_error("Invalid SAN move: " + string(san_move));
                                                        // Castling and promotions are the only cases longer than 2
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
                    default:
                        throw domain_error("Invalid SAN move: " + string(san_move));
                }

                // Trim string from end, again
                move[len-1] = '\0';     // Get rid of 'Q', 'N' etc
                s = move + len-2;
                while( s>=move && !(isascii(*s) && isalnum(*s)) )
                    *s-- = '\0';    // get rid of '=' but not '1','8'
                len = (int)strlen(move);
            }
        }
    }

    // Castling
    if (0 == strcasecmp(move, "oo") || 0 == strcasecmp(move, "o-o")) {
        strcpy(move, white ? "e1g1" :"e8g8");
        len = 4;
        piece = white ? 'K' :'k';
        default_piece = false;
        kcastling = true;
    }
    else if (0 == strcasecmp(move, "ooo") || 0 == strcasecmp(move, "o-o-o")) {
        strcpy(move, white ? "e1c1" : "e8c8");
        len = 4;
        piece = white ? 'K' :'k';
        default_piece = false;
        qcastling = true;
    }

    // Destination square for all except pawn takes pawn (eg "ef")
    if (len == 2 && 'a' <= move[0] && move[0] <= 'h'
                 && 'a' <= move[1] && move[1] <= 'h')
    {
        src_file = move[0];  // eg "ab" pawn takes pawn
        dst_file = move[1];
    }
    else if (len == 3 && 'a' <= move[0] && move[0] <= 'h'
                      && '2' <= move[1] && move[1] <= '7'
                      && 'a' <= move[2] && move[2] <= 'h')
    {
        src_file = move[0];  // eg "a3b"  pawn takes pawn
        dst_file = move[2];
    }
    else if (len >= 2 && 'a' <= move[len-2] && move[len-2] <= 'h'
                      && '1' <= move[len-1] && move[len-1] <= '8')
    {
        dst_file = move[len-2];
        dst_rank = move[len-1];
        dst_ = SQ(dst_file,dst_rank);
    }
    else {
        throw domain_error("Invalid SAN move: " + string(san_move));
    }

    // Source square and or piece
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
                        throw domain_error("Invalid SAN move: " + string(san_move));
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

    // Check against all possible moves
    auto list = position.legal_moves();

    // Have source and destination, eg "d2d3"
    if( enpassant )
        src_rank = dst_rank = '\0';
    if( src_file && src_rank && dst_file && dst_rank )
    {
        for (auto& m : list) {
            if( (default_piece || piece==position.squares[m.src])  &&
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
            if( piece     ==   position.squares[m.src]  &&
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
            if( piece     ==   position.squares[m.src]   &&
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
            if( piece     ==   position.squares[m.src]      &&
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
            if( piece     ==   position.squares[m.src]      &&
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
    else if (dst_rank && dst_file) {
        for (auto& m : list) {
            if (piece == position.squares[m.src] && dst_ == m.dst) {
                found = &m;
                break;
            }
        }
    }

    // Copy found move
    if (!found) {
        throw domain_error("Invalid SAN move: " + string(san_move));
    }

    bool found_promotion =
        ( found->special == SPECIAL_PROMOTION_QUEEN ||
            found->special == SPECIAL_PROMOTION_ROOK ||
            found->special == SPECIAL_PROMOTION_BISHOP ||
            found->special == SPECIAL_PROMOTION_KNIGHT
        );
    if (promotion && !found_promotion)
        throw domain_error("Invalid SAN move: " + string(san_move));
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

    return *found;
}
