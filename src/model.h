// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#ifndef MODEL_H
#define MODEL_H

struct Model;

typedef void (*ModelChanged)(struct Model *model, void *data);

struct Observer {
    struct Observer *next;
    struct Observer *prev;
    ModelChanged     model_changed;
    void            *data;
};

struct Model {
    struct Observer observers;
};

void model_destroy(struct Model *model);
void model_init(struct Model *model);

void model_observe(struct Model *model, ModelChanged model_changed, void *data);
void model_unobserve(struct Model *model, ModelChanged model_changed, void *data);

void model_changed(struct Model *model);

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
