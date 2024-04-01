// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "centaur.h"
#include "cfg.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace thc;

struct Centaur centaur;

//
// Board view
//

class BoardView : public View {
private:
    unique_ptr<Image> pieces;

public:
    BoardView();
    void render(Context& context) override;
};

BoardView::BoardView()
    : pieces{Image::readbmp("assets/pieces.bmp")}
{
    bounds = {0, 0, 128, 128};
    if (!pieces) {
        throw runtime_error("Failed to load pieces.bmp");
    }
}

void BoardView::render(Context& context) {
    const auto sprites     = " PRNBQKprnbqk?! ";
    const auto square_size = 16;

    const auto left = bounds.left;
    const auto top  = bounds.top;
    for (auto r = 0; r != 8; ++r) {
        const auto y_dst = top + r * square_size;

        for (auto c = 0; c != 8; ++c) {
            const auto x_dst = left + c * square_size;
            const auto y_src = (r + c) % 2 ? square_size : 0;  // Square color

            Square square = static_cast<Square>(8 * r + c);
            if (centaur.reversed()) {
                square = rotate_square(square);
            }

            const auto piece  = centaur.game->at(square);
            const auto sprite = strchr(sprites, piece);
            const auto x_src  = (sprite - sprites) * square_size;
            context.drawimage(x_dst, y_dst, *pieces, x_src, y_src, square_size, square_size);
        }
    }
}

//
// Centaur view
//

class CentaurView : public View {
    BoardView board_view;

public:
    CentaurView();
    void render(Context& context) override;
};

CentaurView::CentaurView() {
    bounds = {0, 0, 128, 296};
}

void CentaurView::render(Context& context) {
    board_view.render(context);
}

static CentaurView centaur_view;

//
// Centaur
//

void Centaur::render() {
    screen.render(centaur_view);
}

void Centaur::on_changed(Game&) {
    render();
}

void Centaur::set_game(unique_ptr<Game> game) {
    if (centaur.game) {
        centaur.game->unobserve(this);
    }
    centaur.game = std::move(game);
    centaur.game->observe(this);
}

Centaur::Centaur() {
    set_game(make_unique<Game>());
    clear_feedback();
}

bool Centaur::reversed() const {
    return board.reversed;
}

void Centaur::reversed(bool reversed) {
    board.reversed = reversed;
}

int Centaur::batterylevel() {
    return board.batterylevel();
}

int Centaur::charging() {
    return board.charging();
}

Bitmap Centaur::getstate() {
    return board.getstate();
}

// Extend actions history with any new actions read from board.  Return total
// number of unprocessed actions in history.
int Centaur::update_actions() {
    board.read_actions(actions);
    return actions.size();
}

// Pull any outstanding action events from board and discard them.
void Centaur::purge_actions() {
    while (update_actions()) {
        actions.clear();
    }
}

void Centaur::clear_feedback() {
    board.leds_off();
}

void Centaur::led(Square square) {
    board.led(square);
}

void Centaur::led_from_to(Square from, Square to) {
    board.led_from_to(from, to);
}

void Centaur::show_feedback(Bitmap diff) {
    static auto old_square1 = SQUARE_INVALID;
    static auto old_square2 = SQUARE_INVALID;

    auto square1 = SQUARE_INVALID;
    auto square2 = SQUARE_INVALID;

    switch (__builtin_popcountll(diff)) {
    case 1:
        square1 = static_cast<Square>(__builtin_ctzll(diff));
        if (old_square1 == square1 && old_square2 == SQUARE_INVALID) {
            board.led_flash();
        } else {
            old_square1 = square1;
            old_square2 = SQUARE_INVALID;
            board.led(square1);
        }
        break;
    case 2:
        square1 = static_cast<Square>(__builtin_ctzll(diff));
        square2 = static_cast<Square>(__builtin_ctzll(diff & ~(1ull << square1)));
        if (old_square1 != square1 || old_square2 != square2) {
            old_square1 = square1;
            old_square2 = square2;
            board.led_from_to(square1, square2);
        }
        break;
    case 0:   // No change
    default:  // Too many changes
        old_square1 = SQUARE_INVALID;
        old_square2 = SQUARE_INVALID;
        board.leds_off();
        break;
    }
}

// Try to reconstruct earliest move from provided action history
static bool
history_read_move(
    Game&  game,
    Bitmap boardstate,
    ActionList::const_iterator& begin,  // in/out: Next unused action
    ActionList::const_iterator  end,    // Limit of unused actions
    MoveList&       candidates,
    optional<Move>& takeback)
{
    // Reconstruct boardstate from last known position
    auto state = game.bitmap();

    // History of simulated actions, grows as we consume provided history
    ActionList local_actions;

    // Walk through history updating simulated boardstate, trying to read a
    // move at each step.
    while (begin != end) {
        // Consume next action from history
        const auto action = *begin++;

        // Add action to simulated history
        local_actions.push_back(action);

        // Update boardstate to match simulated action
        if (action.lift != SQUARE_INVALID) {
            state &= ~(1ull << action.lift);
        }
        else if (action.place != SQUARE_INVALID) {
            state |= 1ull << action.place;
        }

        // Are we able to read a move?
        const auto maybe_valid =
            game.read_move(state, local_actions, candidates, takeback);

        if (!candidates.empty() || takeback.has_value()) {
            // Yes, we read a move.
            return true;
        }

        // We can accept an incomplete move only at the very end (otherwise we
        // might imagine a nonsense sequence of incomplete moves and make no
        // progress)
        if (maybe_valid && begin == end && state == boardstate) {
            return true;
        }
    }

    // We got nuthin'
    assert(begin == end);
    return false;
}

bool Centaur::read_move(
    Bitmap          boardstate,
    MoveList&       candidates,
    optional<Move>& takeback)
{
    // Assume caller handles this special case before we ever get here.
    assert(boardstate != Board::STARTING_POSITION);

    // Try to read a move
    auto maybe_valid = game->read_move(boardstate, actions, candidates, takeback);

    if (!candidates.empty() || takeback.has_value()) {
        // 5x5, we won't need to review actions history
        actions.clear();
        clear_feedback();
        return true;
    }
    else if (maybe_valid) {
        // Probably OK, but preserve history just in-case
        clear_feedback();
        return true;
    }

    // Looks like an illegal move.  It most likely *is* an illegal move, but to
    // be sure we'll assume we missed a move and try to recover it.
    assert(!maybe_valid && candidates.empty());
    assert(!takeback);

    // Assume we missed a move.  Revisit actions history to reconstruct.
    //
    // We're looking for the longest "tail" of actions that we can make sense
    // of.  That is, while the oldest actions in our history may be noise of
    // some sort (possibly relating to already processed moves), the newer
    // actions should map out a clear path from the last known board position
    // to the current position.  Otherwise we simply don't have any way to
    // interpret them.
    auto begin = actions.cbegin();
    auto end   = actions.cend();
    for (; begin != end; ++begin) {
        const ActionList tail{begin, end};

        // For any given tail, we'll read and simulate the available moves.
        Game copy{*game};
        auto step_valid = true;

        auto tail_begin = tail.cbegin();
        auto tail_end   = tail.cend();
        while (tail_begin != tail_end) {
            MoveList       local_candidates;
            optional<Move> local_takeback;

            const auto pre_begin = tail_begin;
            step_valid = history_read_move(
                copy,
                boardstate,
                tail_begin,        // in/out: Next unused action
                tail_end,
                local_candidates,
                local_takeback);

            // LOOP VARIANT
            // history_read_move() always consumes at least one action, so
            // we're guaranteed to make progress, and the loop will terminate
            assert(pre_begin != tail_begin);

            if (!step_valid) {
                // Consumed everything but unable to read any moves from history
                assert(tail_begin == tail_end);
                break;
            }

            if (local_candidates.empty() && !local_takeback.has_value()) {
                // Should only get an incomplete move at end of actions history
                assert(tail_begin == tail_end);
                break;
            }

            // N.B., there can be more than one candidate move only in the case
            // of pawn promotion.  Should this happen here, in simulation,
            // assume we promote to queen.  Conveniently, the queen promotion
            // will be the first candidate.  (While it is possible to try each
            // promotion in case any of them yield a valid history, that is an
            // exercise for another day.)

            // Simulate move on local copy of game
            if (local_takeback.has_value() && !local_candidates.empty()) {
                copy.revise_move(*local_takeback, local_candidates.front());
            }
            else if (local_takeback.has_value()) {
                copy.play_takeback(*local_takeback);
            }
            else if (!local_candidates.empty()) {
                copy.play_move(local_candidates.front());
            }
        }

        if (step_valid && copy.bitmap() == boardstate) {
            // Reconstruction makes some kind of sense, we've found our tail
            break;
        }
    }

    // If reconstruction failed--there's no tail that makes any kind of sense--we
    // have to insist the user has made an illegal move
    if (begin == end) {
        // We're out of options and must wait for the board to be restored.  If
        // the boardstate does not differ too much from the last known position,
        // we can provide some feedback.
        show_feedback(game->bitmap() ^ boardstate);
        return false;
    }

    // On the other hand, if reconstruction succeeded, we'll now process the
    // first reconstructed move.  (Subsequent moves will emerge from repeated
    // calls to this method.)

    // Delete "noise" actions preceeding reconstructed tail
    actions.erase(actions.cbegin(), begin);

    begin = actions.cbegin();
    end   = actions.cend();
    maybe_valid = history_read_move(
        *game,
        boardstate,
        begin,       // in/out: Next unused action
        end,
        candidates,
        takeback);

    // Must be true, we just simulated it
    assert(maybe_valid);

    if (!candidates.empty() || takeback.has_value()) {
        // Got reconstructed move, consume actions used in reconstruction
        actions.erase(actions.cbegin(), begin);
    }

    clear_feedback();
    return true;
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
