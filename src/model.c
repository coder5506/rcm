// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "model.h"
#include "list.h"

#include <stddef.h>

#include <gc/gc.h>

void model_init(struct Model *model) {
    model->observers = list_new();
}

void model_destroy(struct Model *model) {
    model_init(model);
}

void model_observe(struct Model *model, ModelChanged model_changed, void *data) {
    struct Observer *observer = GC_MALLOC(sizeof *observer);
    observer->model_changed = model_changed;
    observer->data          = data;
    list_push(model->observers, observer);
}

void model_unobserve(struct Model *model, ModelChanged model_changed, void *data) {
    struct Observer *unobserve = NULL;
    struct List     *begin = model->observers->next;
    for (; !unobserve && begin != model->observers; begin = begin->next) {
        struct Observer *observer = begin->data;
        if (observer->model_changed == model_changed && observer->data == data) {
            list_unlink(begin);
            return;
        }
    }
}

void model_changed(struct Model *model) {
    struct List *begin = model->observers->next;
    for (; begin != model->observers; begin = begin->next) {
        struct Observer *observer = begin->data;
        observer->model_changed(model, observer->data);
    }
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
