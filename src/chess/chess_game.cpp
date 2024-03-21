// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_game.h"
#include "chess.h"
#include "../thc/PrivateChessDefs.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>

std::string& Game::tag(const std::string& key) {
    return tags[key];
}

static void game_changed(Game* game, void* data) {
    (void)data;
    if (game->started > 0) {
        return;
    }

    game->started = time(NULL);
    struct tm timestamp;
    localtime_r(&game->started, &timestamp);

    char date[11];
    strftime(date, 11, "%Y.%m.%d", &timestamp);
    game->tag("Date") = date;
}

static void game_reset(Game* game) {
    game->started  = 0;
    game->rowid    = 0;
    game->settings = "";

    game->tags.clear();
    game->tag("Event")  = "?";
    game->tag("Site")   = "?";
    game->tag("Date")   = "????.??.??";
    game->tag("Round")  = "-";
    game->tag("White")  = "?";
    game->tag("Black")  = "?";
    game->tag("Result") = "*";
}

Game::Game() {
    game_reset(this);
    observe((ModelChanged)game_changed, nullptr);
}

Game::Game(const char* txt) : Game() {
    rules.Forsyth(txt);
}

void Game::apply_move(thc::Move move) {
    rules.PlayMove(move);
}

void Game::apply_takeback(thc::Move takeback) {
    rules.PopMove(takeback);
}

static std::uint64_t white_bitmap(const thc::ChessRules& position) {
    std::uint64_t bitmap{0};
    std::uint64_t mask{1};
    for (thc::Square sq = thc::a8; sq <= thc::h1; ++sq) {
        switch (position.squares[sq]) {
        case 'P': case 'N': case 'B': case 'R': case 'Q': case 'K':
            bitmap |= mask;
            break;
        default:
            break;
        }
        mask <<= 1;
    }
    return bitmap;
}

static std::uint64_t black_bitmap(const thc::ChessRules& position) {
    std::uint64_t bitmap{0};
    std::uint64_t mask{1};
    for (thc::Square sq = thc::a8; sq <= thc::h1; ++sq) {
        switch (position.squares[sq]) {
        case 'p': case 'n': case 'b': case 'r': case 'q': case 'k':
            bitmap |= mask;
            break;
        default:
            break;
        }
        mask <<= 1;
    }
    return bitmap;
}

static std::uint64_t position_bitmap(const thc::ChessRules& position) {
    return white_bitmap(position) | black_bitmap(position);
}

std::uint64_t Game::bitmap() const {
    return position_bitmap(rules);
}

// True if boardstate might represent a transition into this position
static bool position_incomplete(
    const thc::ChessRules& position, std::uint64_t boardstate)
{
    // The boardstate for a move in-progress can differ only in small ways from
    // the boardstate of the completed move:
    // - It can be missing up-to two pieces of the same color, but only when
    //   castling.
    // - It can be missing up-to one piece of each color, but only when capturing.
    // - It can have up-to one extra piece of the opposite color, but only when
    //   capturing en-passant.

    // Present in position but not on board
    const std::uint64_t removed_bmp = position_bitmap(position) & ~boardstate;
    const int removed = __builtin_popcountll(removed_bmp);

    // Present on board but not in position
    const int added   = __builtin_popcountll(boardstate & ~position_bitmap(position));

    // First check: just count the differences
    if (removed > 2 || added > 1 || removed + added > 2) {
        return false;
    }

    // Second check: look at the colors involved
    const int white_removed = __builtin_popcountll(white_bitmap(position) & ~boardstate);
    const int black_removed = __builtin_popcountll(black_bitmap(position) & ~boardstate);
    assert(removed == white_removed + black_removed);

    if ( position.white && black_removed > 1) {
        return false;
    }
    if (!position.white && white_removed > 1) {
        return false;
    }

    if (white_removed == 2) {
        if (position.wking_allowed()  && removed_bmp == (1ull << thc::e1 | 1ull << thc::h1)) {
            return true;
        }
        if (position.wqueen_allowed() && removed_bmp == (1ull << thc::e1 | 1ull << thc::a1)) {
            return true;
        }
        return false;
    }
    if (black_removed == 2) {
        if (position.bking_allowed()  && removed_bmp == (1ull << thc::e8 | 1ull << thc::h8)) {
            return true;
        }
        if (position.bqueen_allowed() && removed_bmp == (1ull << thc::e8 | 1ull << thc::a8)) {
            return true;
        }
        return false;
    }

    // Third check: consider the possibility of en passant
    if (added == 0) {
        // i.e., we're unable to reject the possibility
        return true;
    }

    assert(added == 1);
    if (position.groomed_enpassant_target() == thc::SQUARE_INVALID) {
        return false;
    }

    // // In mailbox coordinates...
    // const int direction = position->turn == 'w' ? -10 : 10;
    // const int captured  = mailbox_index[position->en_passant] - direction;

    // uint64_t mask = 1 << position->en_passant | 1 << board_index[captured];
    // if (board_index[captured-1] != thc::SQUARE_INVALID) {
    //     mask |= 1 << board_index[captured-1];
    // }
    // if (board_index[captured+1] != thc::SQUARE_INVALID) {
    //     mask |= 1 << board_index[captured+1];
    // }

    // const uint64_t diff = position->bitmap ^ boardstate;
    // return (diff & ~mask) == 0;

    return true;
}

// Construct a list of candidate moves in this position that match the given
// boardstate.  The return indicates if there are any viable candidates:
// - true if any candidates are found OR if the boardstate could be in
//   transition to a valid move,
// - false if the boardstate is incompatible with all legal moves in this
//   position.
static bool position_read_moves(
    thc::ChessRules&        before,
    std::uint64_t           boardstate,
    std::vector<thc::Move>& candidates)
{
    bool maybe_valid{false};

    std::vector<thc::Move> legal_moves;
    before.GenLegalMoveList(legal_moves);

    candidates.clear();
    for (auto move : legal_moves) {
        before.PushMove(move);
        if (position_bitmap(before) == boardstate) {
            candidates.push_back(move);
            maybe_valid = true;
        } else {
            maybe_valid = maybe_valid || position_incomplete(before, boardstate);
        }
        before.PopMove(move);
    }

    assert(maybe_valid || candidates.empty());
    return maybe_valid;
}

// The boardstate alone can be ambiguous in the case of piece captures.  Here
// we use the actions history to disambiguate the move.  Note that we still
// generate a list, because (a) the candidates might be promotions which cannot
// be resolved here and (b) the actions history might be incomplete.
bool position_read_move(
    thc::ChessRules&           before,
    std::uint64_t              boardstate,
    std::vector<Action>::const_iterator begin,
    std::vector<Action>::const_iterator end,
    std::vector<thc::Move>&    candidates)
{
    const bool maybe_valid = position_read_moves(before, boardstate, candidates);
    if (candidates.empty()) {
        return maybe_valid;
    }

    assert(maybe_valid);

    // This is a check.  When we're done, the list should contain either moves
    // or promotions, but not both.
    int num_moves{0};
    int num_promotions{0};

    const auto old_candidates{std::move(candidates)};
    for (const auto move : old_candidates) {
        if (move.is_promotion()) {
            // Can't resolve promotion here
            candidates.push_back(move);
            ++num_promotions;
            continue;
        }

        if (move.capture == ' ') {
            // No capture, not ambiguous
            candidates.push_back(move);
            ++num_moves;
            continue;
        }

        // History should show a lift followed by a place on the capture square
        bool got_lift  = false;
        bool got_place = false;
        for (auto p = end; p != begin;) {
            --p;
            if (!got_place) {
                got_place = p->place == move.capture;
            }
            else if (!got_lift) {
                got_lift = p->lift == move.capture;
            }
        }
        if (got_lift && got_place) {
            candidates.push_back(move);
            ++num_moves;
        }
    }

    assert(num_moves == 0 || num_promotions == 0);
    assert(num_moves == 0 || num_moves == 1);
    assert(0 <= num_promotions && num_promotions <= 4);
    assert(candidates.size() == num_moves + num_promotions);
    return maybe_valid;
}

// Here we try to interpret the move at a higher-level than the position,
// taking into account the context of the game.  So yes, the boardstate
// might represent a move or a move in-progress, but it could also be the
// completion of a two-part move (i.e., castling rook first) or a takeback.
bool Game::read_move(
    std::uint64_t              boardstate,
    std::vector<Action>::const_iterator begin,
    std::vector<Action>::const_iterator end,
    std::vector<thc::Move>&    candidates,
    std::optional<thc::Move>&  takeback)
{
    candidates.clear();
    takeback.reset();

    // If boardstate matches current position, there's no move to read.
    if (position_bitmap(rules) == boardstate) {
        // i.e., not an error
        return true;
    }

    // Can boardstate be reached by a legal move?
    bool maybe_valid = position_read_move(rules, boardstate, begin, end, candidates);
    if (maybe_valid) {
        return true;
    }

    // // Is this the completion of a castling move?
    // struct List     *castling = list_new();
    // struct Position *previous = game_previous(game);
    // if (previous) {
    //     generate_castle_moves(castling, previous);
    // }
    // for (struct List *each = castling->next; each != castling; each = each->next) {
    //     struct Position *after = position_apply_move(previous, (struct Move*)each->data);
    //     if (!position_legal(after)) {
    //         continue;
    //     }
    //     if (after->bitmap != boardstate) {
    //         maybe_valid = maybe_valid || position_incomplete(after, boardstate);
    //         continue;
    //     }

    //     // Find takeback move
    //     struct List *it = list_begin(previous->moves_played);
    //     for (; it != list_end(previous->moves_played); it = it->next) {
    //         struct Move *move = (struct Move*)it->data;
    //         if (position_equal(move->after, current)) {
    //             *takeback = move;
    //             break;
    //         }
    //     }
    //     assert(*takeback);

    //     // Undo previous move and apply castling move
    //     list_push(candidates, each->data);

    //     return true;
    // }

    // // Is this a takeback?
    // if (previous && previous->bitmap == boardstate) {
    //     struct List *each = previous->moves_played->next;
    //     for (; each != previous->moves_played; each = each->next) {
    //         struct Move *move = (struct Move*)each->data;
    //         if (position_equal(move->after, current)) {
    //             *takeback = move;
    //             return true;
    //         }
    //     }
    // }

    // // Possibly a takeback in progress?
    // return previous && position_incomplete(previous, boardstate);

    return false;
}

// This file is part of the Raccoon's Centaur Mods (RCM).
//
// RCM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RCM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
