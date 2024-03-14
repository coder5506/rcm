// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "centaur.h"
#include "db.h"
#include "httpd.h"
#include "standard.h"

#include <stdlib.h>

#include <gc/gc.h>
#include <jansson.h>

int main(void) {
    GC_INIT();
    json_set_alloc_funcs(GC_malloc, GC_free);

    if (db_open() != 0) {
        // Required for centaur_open
        return EXIT_FAILURE;
    }
    if (centaur_open() != 0) {
        db_close();
        return EXIT_FAILURE;
    }

    // Optional, we're happy to ignore failure here
    httpd_start();

    // Run the standard gameplay module
    standard_main();

    // Cleanup
    httpd_stop();
    centaur_close();
    db_close();
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
