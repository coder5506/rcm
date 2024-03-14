// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef RCM_MODEL_H
#define RCM_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

struct List;
struct Model;

typedef void (*ModelChanged)(struct Model *model, void *data);

struct Model {
    struct List *observers;
};

void model_destroy(struct Model *model);
void model_init(struct Model *model);

void model_observe(struct Model *model, ModelChanged model_changed, void *data);
void model_unobserve(struct Model *model, ModelChanged model_changed, void *data);

void model_changed(struct Model *model);

//
// Convenience macros
//

#define MODEL_DESTROY(model) model_destroy((struct Model*)model)
#define MODEL_INIT(model) model_init((struct Model*)model)

#define MODEL_OBSERVE(model, model_changed, data) \
model_observe((struct Model*)model, (ModelChanged)model_changed, data)

#define MODEL_UNOBSERVE(model, model_changed, data) \
model_unobserve((struct Model*)model, (ModelChanged)model_changed, data)

#define MODEL_CHANGED(model) model_changed((struct Model*)model)

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
