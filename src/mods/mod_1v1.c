// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "mod_1v1.h"
#include "mod_private.h"
#include "mods.h"

#include <gc/gc.h>

struct Mod1v1 {
    struct CentaurMod mod;
};

static bool
mod_1v1_read_move(
    struct Mod1v1 *mod,
    struct List   *candidates,
    struct Move  **takeback)
{
    (void)mod;
    (void)candidates;
    (void)takeback;

    // Accept any legal move
    return true;
}

static struct ModVTable mod_1v1_vtable = {
    .read_move = (ModReadMove)mod_1v1_read_move,
};

struct CentaurMod *mod_1v1_new(void)
{
    struct Mod1v1 *mod = GC_MALLOC(sizeof *mod);
    mod->mod.vtable = &mod_1v1_vtable;
    return (struct CentaurMod*)mod;
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
