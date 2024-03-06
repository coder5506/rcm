// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "cfg.h"

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <sys/stat.h>

static const char *xdg_data_home(void) {
    static char *data_home = NULL;
    if (data_home) {
        return data_home;
    }

    data_home = getenv("XDG_DATA_HOME");
    if (data_home) {
        return data_home;
    }

    const char *home = getenv("HOME");
    if (home) {
        asprintf(&data_home, "%s/.local", home);
        int err = mkdir(data_home, 0700);
        if (!err || errno == EEXIST) {
            free(data_home);
            data_home = NULL;
            asprintf(&data_home, "%s/.local/share", home);
            err = mkdir(data_home, 0700);
        }
        if (err && errno != EEXIST) {
            free(data_home);
            data_home = NULL;
        }
    }

    if (!data_home) {
        data_home = "/usr/local/share";
    }
    return data_home;
}

const char *cfg_data_home(void) {
    static char *data_home = NULL;
    if (!data_home) {
        asprintf(&data_home, "%s/raccoons-centaur-mods", xdg_data_home());
        int err = mkdir(data_home, 0700);
        if (err && errno != EEXIST) {
            free(data_home);
            data_home = NULL;
        }
    }
    if (!data_home) {
        data_home = "/usr/local/share/raccoons-centaur-mods";
    }
    return data_home;
}

const char *cfg_pgn_dir(void) {
    static char *pgn_dir = NULL;
    if (!pgn_dir) {
        asprintf(&pgn_dir, "%s/pgn", cfg_data_home());
        int err = mkdir(pgn_dir, 0700);
        if (err && errno != EEXIST) {
            free(pgn_dir);
            pgn_dir = NULL;
        }
    }
    if (!pgn_dir) {
        pgn_dir = "/usr/local/share/raccoons-centaur-mods/pgn";
    }
    return pgn_dir;
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
