// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef RCM_KV_H
#define RCM_KV_H

#include "list.h"

struct KeyValue {
    struct KeyValue *next;
    struct KeyValue *prev;
    void            *data;
    const char      *key;
};

static inline struct KeyValue *kv_begin(struct KeyValue *list) {
    return (struct KeyValue*)list_begin((struct List*)list);
}

static inline struct KeyValue *kv_end(struct KeyValue *list) {
    return (struct KeyValue*)list_end((struct List*)list);
}

static inline void
kv_clear(struct KeyValue *list) { list_clear((struct List*)list); }

static inline bool
kv_empty(const struct KeyValue *list) { return list_empty((struct List*)list ); }

static inline bool
kv_valid(const struct KeyValue *list) { return list_valid((struct List*)list); }

struct KeyValue *kv_new();
struct KeyValue *kv_copy(const struct KeyValue *list);

static inline void kv_link(struct KeyValue *node, struct KeyValue *before) {
    list_link((struct List*)node, (struct List*)before);
}

static inline void
kv_unlink(struct KeyValue *node) { list_unlink((struct List*)node); }

void kv_push(struct KeyValue *list, const char *key, void *data);
struct KeyValue *kv_pop(struct KeyValue *list);

void kv_unshift(struct KeyValue *list, const char *key, void *data);
struct KeyValue *kv_shift(struct KeyValue *list);

struct KeyValue *kv_find(const struct KeyValue *list, const char *key);
struct KeyValue *kv_first(const struct KeyValue *list);
struct KeyValue *kv_index(const struct KeyValue *list, int index);
struct KeyValue *kv_last(const struct KeyValue *list);

static inline int kv_length(const struct KeyValue *list) {
    return list_length((struct List*)list);
}

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
