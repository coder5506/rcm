// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "board.h"
#include "boardserial.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <time.h>

const uint64_t STARTING_POSITION = 0xffff00000000ffff;

// Shutdown connection to board
void board_close(void) {
    boardserial_close();
}

// Initialize connection to board
int board_open(void) {
    return boardserial_open();
}

// Display boardstate
void board_printstate(uint64_t boardstate) {
    uint64_t mask = 1;
    for (int r = 0; r != 8; ++r) {
        printf("+---+---+---+---+---+---+---+---+\n");
        for (int c = 0; c != 8; ++c) {
            printf("| %d ", (boardstate & mask) != 0);
        }
        printf("|\n");
        mask <<= 1;
    }
    printf("+---+---+---+---+---+---+---+---+\n");
}

// Read current state of board fields
uint64_t board_getstate(void) {
    return boardserial_boardstate();
}

// Return battery status
int board_batterylevel(void) {
    const int charging = boardserial_chargingstate();
    if (charging == -1) {
        return -1;
    }
    return charging & 31;
}

// Return charging status
int board_charging(void) {
    const int charging = boardserial_chargingstate();
    if (charging == -1) {
        return -1;
    }
    return (charging >> 5 & 7) == 1;
}

void board_leds_off(void) {
    boardserial_leds_off();
}

int board_read_actions(struct Action *actions, int max_actions) {
    assert(actions);
    assert(max_actions >= 1); // Allowing 0 is more trouble than it's worth

    // Ensure all actions are empty
    for (int i = 0; i != max_actions; ++i) {
        actions[i] = EMPTY_ACTION;
    }

    unsigned char buf[256];
    const int num_read = boardserial_readdata(buf, sizeof buf);
    if (num_read <= 6) {
        // Failed to read data packet
        return 0;
    }

    int n = 0;
    for (int i = 5; i < num_read - 1;) {
        if (n == max_actions && (buf[i] == 64 || buf[i] == 65)) {
            // Buffer too small, keep most recent actions
            memcpy(actions, actions + 1, (n - 1) * sizeof *actions);
            --n;
            actions[n] = EMPTY_ACTION;
        }
        assert(0 <= n && n < max_actions);

        switch (buf[i++]) {
        case 64:
            if (square_valid(buf[i])) {
                actions[n].lift  = buf[i++];
                actions[n].place = INVALID_SQUARE;
                ++n;
            }
            break;
        case 65:
            if (square_valid(buf[i])) {
                actions[n].lift  = INVALID_SQUARE;
                actions[n].place = buf[i++];
                ++n;
            }
            break;
        default:
            break;
        }
    }

    assert(0 <= n && n <= max_actions);
    return n;
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
