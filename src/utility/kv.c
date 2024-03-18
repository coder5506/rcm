// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "kv.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static struct KeyValue *node_new(const char *key, void *data) {
    struct KeyValue *node = malloc(sizeof *node);
    *node = (struct KeyValue){.data = data, .key = key};
    return node;
}

struct KeyValue *kv_new() {
    struct KeyValue *list = node_new(NULL, NULL);
    *list = (struct KeyValue){.next = list, .prev = list};
    list->data = list;
    assert(kv_valid(list));
    return list;
}

struct KeyValue *kv_copy(const struct KeyValue *list) {
    struct KeyValue *copy = kv_new();
    for (struct KeyValue *it = list->next; it != list; it = it->next) {
        kv_push(copy, it->key, it->data);
    }
    return copy;
}

void kv_push(struct KeyValue *list, const char *key, void *data) {
    assert(list);
    kv_link(node_new(key, data), list);
}

struct KeyValue *kv_pop(struct KeyValue *list) {
    assert(list);
    struct KeyValue *node = list->prev != list ? list->prev : NULL;
    if (node) {
        kv_unlink(node);
    }
    return node;
}

void kv_unshift(struct KeyValue *list, const char *key, void *data) {
    assert(list);
    kv_link(node_new(key, data), list->next);
}

struct KeyValue *kv_shift(struct KeyValue *list) {
    assert(list);
    struct KeyValue *node = (list->next != list) ? list->next : NULL;
    if (node) {
        kv_unlink(node);
    }
    return node;
}

struct KeyValue *kv_find(const struct KeyValue *list, const char *key) {
    for (struct KeyValue *it = list->next; it != list; it = it->next) {
        if (strcmp(it->key, key) == 0) {
            return it;
        }
    }
    return NULL;
}

struct KeyValue *kv_first(const struct KeyValue *list) {
    return list->next != list ? list->next : NULL;
}

struct KeyValue *kv_index(const struct KeyValue *list, int index) {
    assert(list);

    if (index < 0) {
        struct KeyValue *it = list->prev;
        while (++index < 0 && it != list) {
            it = it->prev;
        }
        return it != list ? it : NULL;
    }

    struct KeyValue *it = list->next;
    while (--index >= 0 && it != list) {
        it = it->next;
    }
    return it != list ? it : NULL;
}

struct KeyValue *kv_last(const struct KeyValue *list) {
    return list->prev != list ? list->prev : NULL;
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
