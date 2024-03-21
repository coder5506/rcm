// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "centaur.h"
#include "cfg.h"
#include "graphics.h"
#include "screen.h"
#include "utility/model.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct Centaur centaur = {0};

//
// Board view
//

static Image* load_pieces() {
    static Image* pieces = NULL;
    if (!pieces) {
        pieces = image_readbmp("assets/pieces.bmp");
    }
    return pieces;
}

static void render_board(View* board_view, Context* context) {
    const auto sprites     = " PRNBQKprnbqk?! ";
    const auto square_size = 16;

    const auto pieces = load_pieces();
    if (!pieces) {
        return;
    }

    const auto left = board_view->bounds.left;
    const auto top  = board_view->bounds.top;
    for (auto r = 0; r != 8; ++r) {
        const auto y_dst = top + r * square_size;

        for (auto c = 0; c != 8; ++c) {
            const auto x_dst = left + c * square_size;
            const auto y_src = (r + c) % 2 ? square_size : 0; // Square color

            thc::Square square = static_cast<thc::Square>(8 * r + c);
            if (board_reversed) {
                square = rotate_square(square);
            }

            const auto piece  = centaur.game->at(square);
            const auto sprite = strchr(sprites, piece);
            const auto x_src  = (sprite - sprites) * square_size;
            graphics_drawimage(
                context, x_dst, y_dst, pieces, x_src, y_src, square_size, square_size);
        }
    }
}

static View board_view = {
    .render = render_board,
    .bounds = {.left = 0, .top = 0, .right = 128, .bottom = 128},
};

//
// Centaur view
//

static void render_centaur(struct View *centaur_view, struct Context *context) {
    (void)centaur_view;
    board_view.render(&board_view, context);
}

static struct View centaur_view = {
    .render = render_centaur,
    .bounds = {.left = 0, .top = 0, .right = 128, .bottom = 296},
};

//
// Centaur
//

void centaur_close() {
    board_close();
    screen_close();
}

void centaur_render() {
    screen_render(centaur.screen_view);
}

static void game_changed(Game* game, void* data) {
    (void)game;
    (void)data;
    centaur_render();
}

void centaur_set_game(Game* game) {
    if (centaur.game) {
        centaur.game->unobserve((ModelChanged)game_changed, NULL);
    }
    centaur.game = game;
    centaur.game->observe((ModelChanged)game_changed, NULL);
}

static void remove_actions(int begin, int end) {
    assert(0 <= begin && begin <= end && end <= centaur.actions.size());
    centaur.actions.erase(centaur.actions.begin() + begin, centaur.actions.begin() + end);
}

static void consume_actions(int num_consume) {
    remove_actions(0, num_consume);
}

void centaur_clear_actions() {
    centaur.actions.clear();
}

// Initialize both field array and screen
int centaur_open() {
    if (board_open() != 0) {
        return 1;
    }
    if (screen_open() != 0) {
        board_close();
        return 1;
    }

    centaur_clear_actions();

    centaur.screen_view = &centaur_view;
    centaur_set_game(new Game);

    return 0;
}

int centaur_batterylevel() {
    return board_batterylevel();
}

int centaur_charging() {
    return board_charging();
}

uint64_t centaur_getstate() {
    return board_getstate();
}

int centaur_update_actions() {
    return board_read_actions(centaur.actions);
}

void centaur_purge_actions() {
    while (centaur_update_actions() > 0) {
        // loop
    }
    centaur_clear_actions();
}

static void clear_feedback() {
    board_leds_off();
}

void centaur_led(thc::Square square) {
    board_led(square);
}

void centaur_led_from_to(thc::Square from, thc::Square to) {
    board_led_from_to(from, to);
}

static void show_feedback(std::uint64_t diff) {
    static auto old_square1 = thc::SQUARE_INVALID;
    static auto old_square2 = thc::SQUARE_INVALID;

    auto square1 = thc::SQUARE_INVALID;
    auto square2 = thc::SQUARE_INVALID;

    switch (__builtin_popcountll(diff)) {
    case 1:
        square1 = static_cast<thc::Square>(__builtin_ctzll(diff));
        if (old_square1 == square1 && old_square2 == thc::SQUARE_INVALID) {
            board_led_flash();
        } else {
            old_square1 = square1;
            old_square2 = thc::SQUARE_INVALID;
            board_led(square1);
        }
        break;
    case 2:
        square1 = static_cast<thc::Square>(__builtin_ctzll(diff));
        square2 = static_cast<thc::Square>(__builtin_ctzll(diff & ~(1ull << square1)));
        if (old_square1 != square1 || old_square2 != square2) {
            old_square1 = square1;
            old_square2 = square2;
            board_led_from_to(square1, square2);
        }
        break;
    case 0:  // No change
    default: // Too many changes
        old_square1 = thc::SQUARE_INVALID;
        old_square2 = thc::SQUARE_INVALID;
        board_leds_off();
        break;
    }
}

static bool
history_read_move(
    Game&         game,
    std::uint64_t boardstate,
    std::vector<Action>::const_iterator& begin, // in/out: Next unused action
    std::vector<Action>::const_iterator  end,   // Limit of unused actions
    std::vector<thc::Move>&   candidates,
    std::optional<thc::Move>& takeback)
{
    // Reconstruct boardstate from last known position
    std::uint64_t state = game.bitmap();
    for (; begin != end; ++begin) {
        if (begin->lift != thc::SQUARE_INVALID) {
            state &= ~(1ull << begin->lift);
        }
        if (begin->place != thc::SQUARE_INVALID) {
            state |= 1ull << begin->place;
        }

        bool maybe_valid = game.read_move(
            state,
            begin,
            end,
            candidates,
            takeback);

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

bool
centaur_read_move(
    Game&                     game,
    std::uint64_t             boardstate,
    std::vector<thc::Move>&   candidates,
    std::optional<thc::Move>& takeback)
{
    // Try to read a move
    bool maybe_valid = game.read_move(
        boardstate,
        centaur.actions.cbegin(),
        centaur.actions.cend(),
        candidates,
        takeback);

    if (!candidates.empty()) {
        // 5x5, we won't need to review actions history
        centaur_clear_actions();
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
    auto begin = centaur.actions.cbegin();
    auto end   = centaur.actions.cend();
    for (; begin != end; ++begin) {
        // TODO save/restore game state!
        bool step_valid = true;
        for (auto i = begin; i != end; ++i) {
            std::vector<thc::Move>   local_candidates;
            std::optional<thc::Move> local_takeback;
            step_valid = history_read_move(
                game, boardstate, i, end, local_candidates, local_takeback);
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
                game.apply_takeback(local_takeback.value());
            }
            if (!local_candidates.empty()) {
                game.apply_move(local_candidates.front());
            }
        }

        if (step_valid && game.bitmap() == boardstate) {
            // OK, reconstruction makes some kind of sense
            break;
        }
    }

    // Reconstruction succeeded, now read the first move.
    if (begin != end) {
        maybe_valid = history_read_move(
            game, boardstate, begin, end, candidates, takeback);

        if (takeback || !candidates.empty()) {
            // Got reconstructed move from actions history
            consume_actions(begin - centaur.actions.cbegin());
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
    show_feedback(game.bitmap() ^ boardstate);
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
