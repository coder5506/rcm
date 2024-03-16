// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef RCM_CHESS_UCI_H
#define RCM_CHESS_UCI_H

#ifdef __cplusplus
extern "C" {
#endif

struct Game;
struct UCIEngine;

struct UCIEngine *uci_execvp(const char *file, char *const argv[]);
void uci_quit(struct UCIEngine *engine);

enum UCIRequest {
    UCI_REQUEST_HINT,
    UCI_REQUEST_PLAY,
    UCI_REQUEST_QUIT,
    UCI_REQUEST_UCI,
};

struct UCIMessage {
    enum UCIRequest type;
};

struct UCIPlayMessage {
    struct UCIMessage  m;
    const struct Game *game;
    int                elo;
    struct Move       *move;
};

// First available response or NULL, does not block
struct UCIMessage *uci_receive(struct UCIEngine *engine);

void uci_send(struct UCIEngine *engine, struct UCIMessage *message);

#ifdef __cplusplus
}
#endif

#endif

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
