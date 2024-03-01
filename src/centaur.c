// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "centaur.h"
#include "graphics.h"
#include "screen.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <time.h>

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
    (void)board_view;

    const char *sprites     = " PRNBQKprnbqk?! ";
    const int   square_size = 16;

    const struct Image    *pieces   = load_pieces();
    const struct Position *position = centaur.game.history.prev;
    if (!pieces || !position) {
        return;
    }

    for (int r = 0; r != 8; ++r) {
        const int y_dst = r * square_size;

        for (int c = 0; c != 8; ++c) {
            const int x_dst = c * square_size;
            const int y_src = (r + c) % 2 ? square_size : 0; // Square color

            const enum Piece piece = position->mailbox.cells[mailbox_index[8*r + c]];
            const char *sprite = strchr(sprites, piece);
            const int   x_src  = (sprite - sprites) * square_size;
            graphics_drawimage(
                context, x_dst, y_dst, pieces, x_src, y_src, square_size, square_size);
        }
    }
}

static struct View board_view = {
    .render = render_board,
};

//
// Utility
//

void sleep_ms(int milliseconds) {
    assert(milliseconds >= 0);
    struct timespec request = {
        .tv_sec  =  milliseconds / 1000,
        .tv_nsec = (milliseconds % 1000) * 1000000,
    };
    nanosleep(&request, NULL);
}

//
// Centaur
//

// Shutdown both field array and screen
void centaur_close(void) {
    screen_close();
    board_close();
}

// Exit cleanly on SIGINT
static void handle_sigint(int signal) {
    (void)signal;
    centaur_close();
    exit(0);
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

    // Register SIGINT handler
    struct sigaction act = {.sa_handler = handle_sigint};
    sigaction(SIGINT, &act, NULL);

    game_from_fen(&centaur.game, NULL);
    centaur.screen_view = &board_view;

    for (int i = 0; i != MAX_ACTIONS; ++i) {
        centaur.actions[i] = EMPTY_ACTION;
    }
    centaur.num_actions = 0;
    return 0;
}

// Put display to sleep
void centaur_sleep(void) {
    screen_sleep();
}

// Wake display from sleep
void centaur_wake(void) {
    screen_wake();
}

// Display boardstate
void centaur_printstate(uint64_t boardstate) {
    board_printstate(boardstate);
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

// Return screen's current graphics context
struct Context *centaur_context(void) {
    return screen_context();
}

// Clear display
void centaur_clear(void) {
    screen_clear();
}

// Render UI to display
void centaur_render(void) {
    screen_render(centaur.screen_view);
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

static void clear_actions(void) {
    consume_actions(centaur.num_actions);
    assert(centaur.num_actions == 0);
}

static void update_actions(void) {
    assert(0 <= centaur.num_actions && centaur.num_actions <= MAX_ACTIONS);

    // Ensure space in buffer, preserving newest actions
    if (centaur.num_actions > 12) {
        consume_actions(centaur.num_actions - 12);
        assert(centaur.num_actions == 12);
    }

    // Read newest actions
    const int num_new = board_read_actions(
        centaur.actions + centaur.num_actions,
        MAX_ACTIONS - centaur.num_actions);
    assert(0 <= num_new && num_new <= MAX_ACTIONS - centaur.num_actions);

    centaur.num_actions += num_new;
    assert(0 <= centaur.num_actions && centaur.num_actions <= MAX_ACTIONS);
}

static void clear_feedback(void) {
    // TODO
}

static void show_feedback(uint64_t diff) {
    // enum Square square1 = INVALID_SQUARE;
    // enum Square square2 = INVALID_SQUARE;

    switch (__builtin_popcountll(diff)) {
    case 1:
        // square1 = __builtin_ctzll(diff);
        break;
    case 2:
        // square1 = __builtin_ctzll(diff);
        // square2 = __builtin_ctzll(diff & ~(1ul << square1));
        break;
    case 0:  // No change
    default: // Too many changes
        clear_feedback();
        break;
    }
}

static struct Position*
history_read_move(
    struct Game  *game,       // Passed to game_read_move
    struct Move **move,       // ...
    struct Move **takeback,   // ...
    bool         *incomplete, // ...
    bool         *promote,    // ...
    uint64_t      boardstate, // Current boardstate
    int          *begin,      // in/out: Next unused action
    int           end)        // Limit of unused actions
{
    assert(game && move && takeback && incomplete && promote && begin);
    assert(0 <= *begin && *begin <= end && end <= centaur.num_actions);

    // Replay actions, ignoring transient errors
    uint64_t state = game->history.prev->bitmap;
    for (int i = *begin; i != end; ++i) {
        if (centaur.actions[i].lift != INVALID_SQUARE) {
            state &= ~(1ul << centaur.actions[i].lift);
        }
        if (centaur.actions[i].place != INVALID_SQUARE) {
            state |= 1ul << centaur.actions[i].place;
        }

        *move       = NULL;
        *takeback   = NULL;
        *incomplete = false;
        *promote    = false;
        struct Position *position = game_read_move(
            game, state, move, takeback, incomplete, promote);
        if (position || (*incomplete && state == boardstate)) {
            *begin = i + 1;
            return position;
        }
    }

    *begin = end;
    return NULL;
}

struct Position*
centaur_read_move(
    struct Game  *game,
    struct Move **move,
    struct Move **takeback,
    bool         *promote)
{
    assert(game);
    assert(move && !*move);
    assert(takeback && !*takeback);
    assert(promote && !*promote);

    // Update history before reading boardstate.  We don't want actions
    // that are newer than the boardstate, but we can handle possibly not
    // having the very latest action.
    update_actions();
    const uint64_t boardstate = centaur_getstate();

    // Try to read a move
    bool incomplete = false;
    struct Position *position = game_read_move(
        game, boardstate, move, takeback, &incomplete, promote);

    // Easy cases:
    if (position) {
        // 5x5, we won't need to review actions history
        clear_actions();
        clear_feedback();
        return position;
    }
    if (incomplete) {
        // Probably OK, but preserve history just in-case
        clear_feedback();
        return NULL;
    }

    // Looks like an illegal move, but we'll try to recover
    assert(!position && !incomplete);

    // Revisit actions history to reconstruct
    int begin = 0; // Want to collect longest tail of usable actions
    int end   = centaur.num_actions;
    for (; begin != end; ++begin) {
        // Replay actions, ignoring transient errors
        struct Game copy;
        game_from_position(&copy, game->history.prev);
        for (int i = begin; i != end;) {
            position = history_read_move(
                &copy, move, takeback, &incomplete, promote, boardstate, &i, end);
            if (position) {
                // Ignore errors here.  If game doesn't update, we just continue.
                if (*move)     { game_move(&copy, *move);         }
                if (*takeback) { game_takeback(&copy, *takeback); }
            }
        }
        const uint64_t state = copy.history.prev->bitmap;
        game_destroy(&copy);

        // N.B., these are the results from the very last iteration
        if ((position || incomplete) && state == boardstate) {
            // Success, [begin, end) makes some kind of sense
            break;
        }
    }

    // Reconstruction succeeded, now read the first move.
    if (begin != end) {
        assert(position || incomplete);
        position = history_read_move(
            game, move, takeback, &incomplete, promote, boardstate, &begin, end);
        if (position ||
            // Only accept an incomplete move at the very end
            (incomplete && begin == centaur.num_actions - 1))
        {
            consume_actions(begin);
            // N.B., not clearing feedback here, that can wait until
            // we've processed all available moves.
            return position;
        }

        // Reconstruction succeeded, why couldn't we read a move?
        assert(false);
    }

    // TODO Everything has failed so far, does board match any known position?

    // We're out of options and must wait for the board to be restored.  If
    // the boardstate does not differ too much from the last known position,
    // we can provide some feedback.
    show_feedback(game->history.prev->bitmap ^ boardstate);
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
