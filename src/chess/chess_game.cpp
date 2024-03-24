// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_game.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <sstream>

std::string& Game::tag(const std::string& key) {
    return tags[key];
}

PositionPtr Game::current() const {
    return history.back();
}

PositionPtr Game::previous() const {
    return history.size() > 1 ? history.at(history.size() - 2) : nullptr;
}

const MoveList& Game::legal_moves() const {
    return current()->legal_moves();
}

std::string Game::fen() const {
    return current()->fen();
}

char Game::at(thc::Square square) const {
    return current()->at(square);
}

void Game::model_changed(Game&) {
    if (started > 0) {
        return;
    }

    started = std::time(nullptr);
    std::ostringstream date;
    date << std::put_time(std::localtime(&started), "%Y.%m.%d");
    tag("Date") = date.str();
}

static void game_reset(Game* game) {
    game->history.clear();
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

Game::Game(const char* txt) {
    game_reset(this);
    this->observe(this);
    history.push_back(std::make_shared<Position>(txt));
}

void Game::apply_move(thc::Move move) {
    history.push_back(current()->apply_move(move));
}

void Game::apply_takeback(thc::Move takeback) {
    const auto after = previous()->move_played(takeback);
    if (after) {
        history.pop_back();
    }
}

void Game::play_move(thc::Move move) {
    apply_move(move);
}

void Game::play_uci_move(std::string_view uci_move) {
    thc::Move move;
    move.TerseIn(const_cast<Position*>(current().get()), uci_move.data());
    play_move(move);
}

// Here we try to interpret the move at a higher-level than the position,
// taking into account the context of the game.  So yes, the boardstate
// might represent a move or a move in-progress, but it could also be the
// completion of a two-part move (i.e., castling rook first) or a takeback.
bool Game::read_move(
    Bitmap            boardstate,
    const ActionList& actions,
    MoveList&         candidates,
    std::optional<thc::Move>& takeback)
{
    candidates.clear();
    takeback.reset();

    // If boardstate matches current position, there's no move to read.
    if (bitmap() == boardstate) {
        // i.e., not an error
        return true;
    }

    // Can boardstate be reached by a legal move?
    bool maybe_valid = current()->read_move(boardstate, actions, candidates);
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
