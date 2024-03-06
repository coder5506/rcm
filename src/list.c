// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "list.h"

#include <assert.h>
#include <stddef.h>

#include <gc/gc.h>

static struct List *node_new(void *data) {
    struct List *node = GC_MALLOC(sizeof *node);
    *node = (struct List){.data = data};
    return node;
}

struct List *list_new() {
    struct List *list = node_new(NULL);
    *list = (struct List){.next = list, .prev = list};
    list->data = list;
    assert(list_valid(list));
    return list;
}

void list_link(struct List *node, struct List *before) {
    assert(node && !node->next && !node->prev);
    assert(before && before != node);

    node->next = before;
    node->prev = before->prev;
    before->prev->next = node;
    before->prev = node;
}

void list_unlink(struct List *node) {
    assert(node);
    assert(node->next && node->next != node);
    assert(node->prev && node->prev != node);

    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = NULL;
    node->prev = NULL;
}

void list_push(struct List *list, void *data) {
    assert(list);
    list_link(node_new(data), list);
}

void *list_pop(struct List *list) {
    assert(list);
    struct List *node = list->prev != list ? list->prev : NULL;
    if (node) {
        list_unlink(node);
    }
    return node->data;
}

void list_unshift(struct List *list, void *data) {
    assert(list);
    list_link(node_new(data), list->next);
}

void *list_shift(struct List *list) {
    assert(list);
    struct List *node = (list->next != list) ? list->next : NULL;
    if (node) {
        list_unlink(node);
    }
    return node->data;
}

struct List *list_find(const struct List *list, void *data) {
    for (struct List *it = list->next; it != list; it = it->next) {
        if (it->data == data) {
            return it;
        }
    }
    return NULL;
}

void *list_first(const struct List *list) {
    return list->next != list ? list->next->data : NULL;
}

void *list_index(const struct List *list, int index) {
    assert(list);

    if (index < 0) {
        struct List *it = list->prev;
        while (++index < 0 && it != list) {
            it = it->prev;
        }
        return it != list ? it->data : NULL;
    }

    struct List *it = list->next;
    while (--index >= 0 && it != list) {
        it = it->next;
    }
    return it != list ? it->data : NULL;
}

void *list_last(const struct List *list) {
    return list->prev != list ? list->prev->data : NULL;
}

int list_length(const struct List *list) {
    int length = 0;
    for (struct List *it = list->next; it != list; it = it->next) {
        ++length;
    }
    return length;
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
