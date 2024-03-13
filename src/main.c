// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "centaur.h"
#include "db.h"
#include "httpd.h"
#include "standard.h"

#include <stdlib.h>

#include <gc/gc.h>

int main(void) {
    GC_INIT();

    if (db_open() != 0) {
        return EXIT_FAILURE;
    }
    if (centaur_open() != 0) {
        db_close();
        return EXIT_FAILURE;
    }
    httpd_start();

    standard_main();

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
