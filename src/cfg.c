// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "cfg.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

    if (getuid() == 0) {
        data_home = "/usr/local/share";
        return data_home;
    }

    const char *home = getenv("HOME");
    if (home) {
        asprintf(&data_home, "%s/.local", home);
        int err = mkdir(data_home, 0750);
        if (!err || errno == EEXIST) {
            free(data_home);
            data_home = NULL;
            asprintf(&data_home, "%s/.local/share", home);
            err = mkdir(data_home, 0750);
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

const char *cfg_data_dir(void) {
    static char *data_dir = NULL;
    if (!data_dir) {
        asprintf(&data_dir, "%s/rcm", xdg_data_home());
        int err = mkdir(data_dir, 0755);
        if (err && errno != EEXIST) {
            free(data_dir);
            data_dir = NULL;
        }
    }
    if (!data_dir) {
        data_dir = "/usr/local/share/rcm";
    }
    return data_dir;
}

int cfg_port(void) {
    const char *s_port = getenv("PORT");
    return s_port ? atoi(s_port) : 80;
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
