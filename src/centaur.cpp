// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "centaur.h"
#include "cfg.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct Centaur centaur;

//
// Board view
//

class BoardView : public View {
private:
    std::unique_ptr<Image> pieces;

public:
    BoardView();
    void render(Context& context) override;
};

BoardView::BoardView()
    : pieces{Image::readbmp("assets/pieces.bmp")}
{
    bounds = {0, 0, 128, 128};
    if (!pieces) {
        throw std::runtime_error("Failed to load pieces.bmp");
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

            thc::Square square = static_cast<thc::Square>(8 * r + c);
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

void Centaur::model_changed(Game&) {
    render();
}

void Centaur::set_game(std::unique_ptr<Game> game) {
    if (centaur.game) {
        centaur.game->unobserve(this);
    }
    centaur.game = std::move(game);
    centaur.game->observe(this);
}

Centaur::Centaur() {
    set_game(std::make_unique<Game>());
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

int Centaur::update_actions() {
    return board.read_actions(actions);
}

void Centaur::purge_actions() {
    actions.clear();
    while (update_actions()) {
        actions.clear();
    }
}

void Centaur::clear_feedback() {
    board.leds_off();
}

void Centaur::led(thc::Square square) {
    board.led(square);
}

void Centaur::led_from_to(thc::Square from, thc::Square to) {
    board.led_from_to(from, to);
}

void Centaur::show_feedback(Bitmap diff) {
    static auto old_square1 = thc::SQUARE_INVALID;
    static auto old_square2 = thc::SQUARE_INVALID;

    auto square1 = thc::SQUARE_INVALID;
    auto square2 = thc::SQUARE_INVALID;

    switch (__builtin_popcountll(diff)) {
    case 1:
        square1 = static_cast<thc::Square>(__builtin_ctzll(diff));
        if (old_square1 == square1 && old_square2 == thc::SQUARE_INVALID) {
            board.led_flash();
        } else {
            old_square1 = square1;
            old_square2 = thc::SQUARE_INVALID;
            board.led(square1);
        }
        break;
    case 2:
        square1 = static_cast<thc::Square>(__builtin_ctzll(diff));
        square2 = static_cast<thc::Square>(__builtin_ctzll(diff & ~(1ull << square1)));
        if (old_square1 != square1 || old_square2 != square2) {
            old_square1 = square1;
            old_square2 = square2;
            board.led_from_to(square1, square2);
        }
        break;
    case 0:  // No change
    default: // Too many changes
        old_square1 = thc::SQUARE_INVALID;
        old_square2 = thc::SQUARE_INVALID;
        board.leds_off();
        break;
    }
}

static bool
history_read_move(
    Game&  game,
    Bitmap boardstate,
    std::vector<Action>::const_iterator& begin, // in/out: Next unused action
    std::vector<Action>::const_iterator  end,   // Limit of unused actions
    std::vector<thc::Move>&   candidates,
    std::optional<thc::Move>& takeback)
{
    // Reconstruct boardstate from last known position
    auto state = game.bitmap();
    for (; begin != end; ++begin) {
        if (begin->lift != thc::SQUARE_INVALID) {
            state &= ~(1ull << begin->lift);
        }
        if (begin->place != thc::SQUARE_INVALID) {
            state |= 1ull << begin->place;
        }

        ActionList local_actions{begin, end};
        auto maybe_valid = game.read_move(state, local_actions, candidates, takeback);

        if (!candidates.empty()) {
            // We got a move, great!
            ++begin;
            return true;
        }

        // We can accept an incomplete move only at the very end
        if (maybe_valid && begin + 1 == end && state == boardstate) {
            ++begin;
            return true;
        }
    }

    // Yeah, we got nuthin'
    begin = end;
    return false;
}

bool Centaur::read_move(
    Bitmap                    boardstate,
    std::vector<thc::Move>&   candidates,
    std::optional<thc::Move>& takeback)
{
    // Try to read a move
    auto maybe_valid = game->read_move(boardstate, actions, candidates, takeback);

    if (!candidates.empty()) {
        // 5x5, we won't need to review actions history
        actions.clear();
        clear_feedback();
        return true;
    } else if (maybe_valid) {
        // Probably OK, but preserve history just in-case
        clear_feedback();
        return true;
    }

    // Looks like an illegal move, but we'll try to recover
    assert(!maybe_valid && candidates.empty());
    assert(!takeback);

    // Assume we missed a move.  Revisit actions history to reconstruct.
    auto begin = actions.cbegin();
    auto end   = actions.cend();
    for (; begin != end; ++begin) {
        // TODO save/restore game state!
        bool step_valid = true;
        for (auto i = begin; i != end; ++i) {
            std::vector<thc::Move>   local_candidates;
            std::optional<thc::Move> local_takeback;
            step_valid = history_read_move(
                *game, boardstate, i, end, local_candidates, local_takeback);
            if (!step_valid) {
                // This is going nowhere
                break;
            }
            if (!local_takeback && local_candidates.empty()) {
                // Should only happen at end of actions history
                assert(i == end);
                break;
            }

            // Simulate move
            if (local_takeback) {
                game->play_takeback(local_takeback.value());
            }
            if (!local_candidates.empty()) {
                game->play_move(local_candidates.front());
            }
        }

        if (step_valid && game->bitmap() == boardstate) {
            // OK, reconstruction makes some kind of sense
            break;
        }
    }

    // Reconstruction succeeded, now read the first move.
    if (begin != end) {
        maybe_valid = history_read_move(
            *game, boardstate, begin, end, candidates, takeback);

        if (takeback || !candidates.empty()) {
            // Got reconstructed move from actions history
            actions.erase(actions.cbegin(), begin);
            clear_feedback();
            return true;
        }

        if (maybe_valid && begin == end) {
            clear_feedback();
            return true;
        }

        // Reconstruction succeeded, so what went wrong?
        assert(false);
    }

    // We're out of options and must wait for the board to be restored.  If
    // the boardstate does not differ too much from the last known position,
    // we can provide some feedback.
    show_feedback(game->bitmap() ^ boardstate);
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
