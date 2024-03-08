// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "centaur.h"
#include "httpd.h"

#include <stdlib.h>

#include <signal.h>

#include <gc/gc.h>

static void cleanup(void) {
    httpd_stop();
    centaur_close();
}

static void handle_sigint(int signal) {
    (void)signal;
    cleanup();
    exit(EXIT_SUCCESS);
}

int main(void) {
    GC_INIT();

    struct sigaction action = {.sa_handler = handle_sigint};
    sigaction(SIGINT, &action, NULL);

    if (centaur_open() != 0) {
        return EXIT_FAILURE;
    }
    if (httpd_start() != 0) {
        centaur_close();
        return EXIT_FAILURE;
    }

    // Reset display and wait for full refresh
    centaur_wake();
    centaur_clear();
    sleep_ms(3000);

    // Display something so we know it's alive
    // centaur_render();
    // sleep_ms(2000);

    centaur_main();
    cleanup();
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
