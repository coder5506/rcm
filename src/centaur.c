// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#include "centaur.h"
#include "board.h"
#include "graphics.h"
#include "screen.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <time.h>

struct Centaur centaur = {0};

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

// Utility
void sleep_ms(int milliseconds) {
    assert(milliseconds >= 0);
    struct timespec request = {
        .tv_sec  =  milliseconds / 1000,
        .tv_nsec = (milliseconds % 1000) * 1000000,
    };
    nanosleep(&request, NULL);
}

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

int centaur_open(void) {
    if (board_open() != 0) {
        return 1;
    }
    if (screen_open() != 0) {
        board_close();
        return 1;
    }

    // Register SIGINT handler
    struct sigaction act = {
        .sa_handler = handle_sigint,
    };
    sigaction(SIGINT, &act, NULL);

    game_init(&centaur.game, NULL);
    centaur.screen_view = &board_view;
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
