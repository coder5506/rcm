// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "centaur.h"
#include "cfg.h"
#include "graphics.h"
#include "screen.h"
#include "utility/list.h"
#include "utility/model.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Centaur centaur = {0};

//
// Board view
//

static struct Image *load_pieces() {
    static struct Image *pieces = NULL;
    if (!pieces) {
        pieces = image_readbmp("assets/pieces.bmp");
    }
    return pieces;
}

static void render_board(struct View *board_view, struct Context *context) {
    const char *sprites     = " PRNBQKprnbqk?! ";
    const int   square_size = 16;

    const struct Image    *pieces   = load_pieces();
    const struct Position *position = game_current(centaur.game);
    if (!pieces || !position) {
        return;
    }

    const int left = board_view->bounds.left;
    const int top  = board_view->bounds.top;
    for (int r = 0; r != 8; ++r) {
        const int y_dst = top + r * square_size;

        for (int c = 0; c != 8; ++c) {
            const int x_dst = left + c * square_size;
            const int y_src = (r + c) % 2 ? square_size : 0; // Square color

            enum Square square = 8 * r + c;
            if (board_reversed) {
                square = rotate_square(square);
            }

            const enum Piece piece = position->mailbox.cells[mailbox_index[square]];
            // printf(" %c", piece);
            const char *sprite = strchr(sprites, piece);
            const int   x_src  = (sprite - sprites) * square_size;
            graphics_drawimage(
                context, x_dst, y_dst, pieces, x_src, y_src, square_size, square_size);
        }
        // printf("\n");
    }
}

static struct View board_view = {
    .render = render_board,
    .bounds = {.left = 0, .top = 0, .right = 128, .bottom = 128},
};

//
// PGN view
//

static void render_pgn(struct View *pgn_view, struct Context *context) {
    const int line_height = context->font->Height;
    const int num_lines = (pgn_view->bounds.bottom - pgn_view->bounds.top) / line_height;

    struct List *pgn_list = game_pgn_list(centaur.game);
    if (!pgn_list) {
        return;
    }

    // Keep only the last N lines
    int num_items = list_length(pgn_list);
    while (num_items > num_lines) {
        list_shift(pgn_list);
        --num_items;
    }

    int left = pgn_view->bounds.left;
    int top  = pgn_view->bounds.top;
    struct List *each = list_begin(pgn_list);
    for (; each != list_end(pgn_list); each = each->next) {
        graphics_drawstring(context, left, top, each->data);
        top += line_height;
    }
}

static struct View pgn_view = {
    .render = render_pgn,
    .bounds = {.left = 0, .top = 128, .right = 128, .bottom = 296},
};

//
// Centaur view
//

static void render_centaur(struct View *centaur_view, struct Context *context) {
    (void)centaur_view;
    (void)&pgn_view;
    board_view.render(&board_view, context);
    // pgn_view.render(&pgn_view, context);
}

static struct View centaur_view = {
    .render = render_centaur,
    .bounds = {.left = 0, .top = 0, .right = 128, .bottom = 296},
};

//
// Centaur
//

// Shutdown both field array and screen
void centaur_close(void) {
    screen_close();
    board_close();
}

static void remove_actions(int begin, int end) {
    assert(0 <= begin && begin <= end && end <= centaur.num_actions);

    memcpy(
        centaur.actions + begin,
        centaur.actions + end,
        (MAX_ACTIONS - end) * sizeof *centaur.actions);

    centaur.num_actions -= end - begin;
    assert(0 <= centaur.num_actions && centaur.num_actions <= MAX_ACTIONS);

    for (int i = centaur.num_actions; i != MAX_ACTIONS; ++i) {
        centaur.actions[i] = EMPTY_ACTION;
    }
}

static void consume_actions(int num_consume) {
    const int num_actions = centaur.num_actions;
    remove_actions(0, num_consume);
    assert(centaur.num_actions == num_actions - num_consume);
}

void centaur_clear_actions(void) {
    consume_actions(centaur.num_actions);
    assert(centaur.num_actions == 0);
}

static void game_changed(struct Game *game, void *data) {
    (void)data;

    centaur_render();
    if (!game->id) {
        return;
    }

    // char *pgn = NULL;
    // asprintf(&pgn, "%s/%li.pgn", cfg_pgn_dir(), game->started);
    // game_save_pgn(game, pgn);
    // free(pgn);

    // char *latest = NULL;
    // asprintf(&latest, "%s/latest", cfg_data_dir());
    // FILE *fp = fopen(latest, "w");
    // free(latest);
    // if (fp) {
    //     fprintf(fp, "%li\n", game->started);
    //     fclose(fp);
    // }

    char *fen = NULL;
    asprintf(&fen, "%s/fen", cfg_data_dir());
    game_save_fen(game, fen);
    free(fen);
}

// Initialize both field array and screen
int centaur_open(void) {
    if (board_open() != 0) {
        return 1;
    }
    if (screen_open() != 0) {
        board_close();
        return 1;
    }

    centaur.game = game_from_fen(NULL);
    centaur.screen_view = &centaur_view;
    MODEL_OBSERVE(centaur.game, game_changed, NULL);

    centaur.num_actions = MAX_ACTIONS;
    centaur_clear_actions();

    return 0;
}

// Read current state of board fields
// MSB: H1=63 G1 F1 ... A1, H2 G2 ... A2, ..., H8 G8 ... A8=0
uint64_t centaur_getstate(void) {
    return board_getstate();
}

int centaur_batterylevel(void) {
    return board_batterylevel();
}

int centaur_charging(void) {
    return board_charging();
}

// Render UI to display
void centaur_render(void) {
    screen_render(centaur.screen_view);
}

int centaur_update_actions(void) {
    assert(0 <= centaur.num_actions && centaur.num_actions <= MAX_ACTIONS);

    // Ensure space in buffer, preserving newest actions
    const int high_water = MAX_ACTIONS - MAX_ACTIONS / 4;
    if (centaur.num_actions > high_water) {
        consume_actions(centaur.num_actions - high_water);
        assert(centaur.num_actions == high_water);
    }

    // Read newest actions
    const int num_new = board_read_actions(
        centaur.actions + centaur.num_actions,
        MAX_ACTIONS - centaur.num_actions);
    assert(0 <= num_new && num_new <= MAX_ACTIONS - centaur.num_actions);

    centaur.num_actions += num_new;
    assert(0 <= centaur.num_actions && centaur.num_actions <= MAX_ACTIONS);

    return num_new;
}

static void clear_feedback(void) {
    board_leds_off();
}

void centaur_led(enum Square square) {
    board_led(square);
}

static void show_feedback(uint64_t diff) {
    static enum Square old_square1 = INVALID_SQUARE;
    static enum Square old_square2 = INVALID_SQUARE;

    enum Square square1 = INVALID_SQUARE;
    enum Square square2 = INVALID_SQUARE;

    switch (__builtin_popcountll(diff)) {
    case 1:
        square1 = __builtin_ctzll(diff);
        if (old_square1 == square1 && old_square2 == INVALID_SQUARE) {
            board_led_flash();
        } else {
            old_square1 = square1;
            old_square2 = INVALID_SQUARE;
            board_led(square1);
        }
        break;
    case 2:
        square1 = __builtin_ctzll(diff);
        square2 = __builtin_ctzll(diff & ~(1ull << square1));
        if (old_square1 != square1 || old_square2 != square2) {
            old_square1 = square1;
            old_square2 = square2;
            board_led_from_to(square1, square2);
        }
        break;
    case 0:  // No change
    default: // Too many changes
        old_square1 = INVALID_SQUARE;
        old_square2 = INVALID_SQUARE;
        board_leds_off();
        break;
    }
}

static bool
history_read_move(
    struct List  *candidates, // Passed to game_read_move
    struct Move **takeback,   // ...
    struct Game  *game,       // ...
    uint64_t      boardstate, // Current boardstate for comparison
    int          *begin,      // in/out: Next unused action
    int           end)        // Limit of unused actions
{
    assert(candidates && takeback && game && begin);
    assert(0 <= *begin && *begin <= end && end <= centaur.num_actions);

    // Replay actions, ignoring transient errors
    struct Position *current = game_current(game);

    // Reconstruct boardstate from last known position
    uint64_t state = current->bitmap;
    for (int i = *begin; i != end; ++i) {
        if (centaur.actions[i].lift != INVALID_SQUARE) {
            state &= ~(1ull << centaur.actions[i].lift);
        }
        if (centaur.actions[i].place != INVALID_SQUARE) {
            state |= 1ull << centaur.actions[i].place;
        }

        bool maybe_valid = game_read_move(
            candidates,
            takeback,
            game,
            state,
            centaur.actions + *begin,
            (i + 1) - *begin);

        if (list_length(candidates) > 0) {
            // We got a move, great!
            *begin = i + 1;
            return true;
        }

        // We can accept an incomplete move only at the very end
        if (maybe_valid && i == end - 1 && state == boardstate) {
            *begin = i + 1;
            return true;
        }
    }

    // Yeah, we got nuthin'
    *begin = end;
    return false;
}

bool
centaur_read_move(
    struct List  *candidates,
    struct Move **takeback,
    struct Game  *game,
    uint64_t      boardstate)
{
    assert(candidates && list_empty(candidates));
    assert(takeback && !*takeback);
    assert(game);

    // Try to read a move
    bool maybe_valid = game_read_move(
        candidates,
        takeback,
        game,
        boardstate,
        centaur.actions,
        centaur.num_actions);

    if (list_length(candidates) > 0) {
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
    assert(!maybe_valid && list_length(candidates) == 0);
    assert(!*takeback);

    // Assume we missed a move.  Revisit actions history to reconstruct.
    int begin = 0; // Want to collect longest tail of usable actions
    int end   = centaur.num_actions;
    for (; begin != end; ++begin) {
        // Replay actions, ignoring transient errors
        struct Game *copy = game_fork(game);
        bool step_valid = true;
        for (int i = begin; i != end;) {
            struct List *local_candidates = list_new();
            struct Move *local_takeback   = NULL;
            step_valid = history_read_move(
                local_candidates, &local_takeback, copy, boardstate, &i, end);
            if (!step_valid) {
                // This is going nowhere
                break;
            }
            if (!local_takeback && list_empty(local_candidates)) {
                // Should only happen at end of actions history
                assert(i == end);
                break;
            }

            // Simulate move
            int err = 0;
            if (local_takeback) {
                err = game_apply_takeback(copy, local_takeback);
            }
            if (!err && !list_empty(local_candidates)) {
                err = game_apply_move(copy, list_first(local_candidates));
            }
            if (err) {
                step_valid = false;
                break;
            }
        }

        if (step_valid && game_current(copy)->bitmap == boardstate) {
            // OK, reconstruction makes some kind of sense
            break;
        }
    }

    // Reconstruction succeeded, now read the first move.
    if (begin != end) {
        maybe_valid = history_read_move(
            candidates, takeback, game, boardstate, &begin, end);

        if (*takeback || !list_empty(candidates)) {
            // Got reconstructed move from actions history
            consume_actions(begin);
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
    show_feedback(game_current(game)->bitmap ^ boardstate);
    return NULL;
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
