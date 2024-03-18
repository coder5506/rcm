// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "model.h"
#include "list.h"

#include <assert.h>
#include <stdlib.h>

struct Observer {
    ModelChanged  model_changed;
    void         *data;
};

void model_init(struct Model *model) {
    assert(model);
    model->observers = list_new();
}

void model_observe(struct Model *model, ModelChanged model_changed, void *data) {
    assert(model && list_valid(model->observers));
    assert(model_changed);

    struct Observer *observer = (struct Observer*)malloc(sizeof *observer);
    observer->model_changed = model_changed;
    observer->data          = data;
    list_push(model->observers, observer);
}

void model_unobserve(struct Model *model, ModelChanged model_changed, void *data) {
    assert(!model || list_valid(model->observers));
    assert(model_changed);

    if (!model) {
        return;
    }

    struct List *begin = list_begin(model->observers);
    for (; begin != list_end(model->observers); begin = begin->next) {
        struct Observer *observer = (struct Observer*)begin->data;
        if (observer->model_changed == model_changed && observer->data == data) {
            list_unlink(begin);
            return;
        }
    }
}

void model_changed(struct Model *model) {
    assert(model && list_valid(model->observers));

    struct List *begin = list_begin(model->observers);
    for (; begin != list_end(model->observers); begin = begin->next) {
        struct Observer *observer = (struct Observer*)begin->data;
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
