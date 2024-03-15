// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_engine.h"
#include "chess.h"

#include <gc/gc.h>

static struct UCIEngine *engine = NULL;

static struct UCIEngine *start_engine(void) {
    if (!engine) {
        char *argv[] = {"stockfish", NULL};
        engine = uci_execvp("stockfish", argv);
    }
    return engine;
}

static struct UCIEngine *get_engine(const char *name) {
    (void)name;
    return start_engine();
}

struct Move *engine_move(struct Game *game, const char *engine_name) {
    (void)game;

    struct UCIEngine *engine = get_engine(engine_name);
    if (!engine) {
        return NULL;
    }

    struct UCIMessage *response = uci_receive(engine);
    if (!response || response->type != UCI_REQUEST_PLAY) {
        return NULL;
    }

    return ((struct UCIPlayMessage*)response)->move;
}

void engine_play(struct Game *game, const char *engine_name, int elo) {
    struct UCIEngine *engine = get_engine(engine_name);
    if (!engine) {
        return;
    }

    struct UCIPlayMessage *request = GC_MALLOC(sizeof *request);
    request->m.type = UCI_REQUEST_PLAY;
    request->game = game;
    request->elo = elo;
    request->move = NULL;

    uci_send(engine, (struct UCIMessage*)request);
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
