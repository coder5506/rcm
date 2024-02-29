// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#include "model.h"
#include "list.h"

#include <stdlib.h>

void model_init(struct Model *model) {
    model->observers = (struct Observer)LIST_INIT(model->observers);
}

void model_destroy(struct Model *model) {
    struct Observer *begin = model->observers.next;
    while (begin != &model->observers) {
        struct Observer *observer = begin;
        begin = begin->next;
        free(observer);
    }
    model_init(model);
}

void model_observe(struct Model *model, ModelChanged model_changed, void *data) {
    struct Observer *observer = malloc(sizeof *observer);
    observer->model_changed = model_changed;
    observer->data          = data;
    list_push((struct Node*)&model->observers, (struct Node*)observer);
}

void model_unobserve(struct Model *model, ModelChanged model_changed, void *data) {
    struct Observer *unobserve = NULL;
    struct Observer *begin = model->observers.next;
    for (; !unobserve && begin != &model->observers; begin = begin->next) {
        if (begin->model_changed == model_changed && begin->data == data) {
            unobserve = begin;
        }
    }

    if (unobserve) {
        unobserve->next->prev = unobserve->prev;
        unobserve->prev->next = unobserve->next;
        free(unobserve);
    }
}

void model_changed(struct Model *model) {
    struct Observer *begin = model->observers.next;
    for (; begin != &model->observers; begin = begin->next) {
        begin->model_changed(model, begin->data);
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
