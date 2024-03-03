// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#include "list.h"

#include <assert.h>
#include <stddef.h>

#include <gc/gc.h>

static void list_insert(struct Node *node, struct Node *before) {
    assert(node && !node->next && !node->prev);
    assert(before && before != node);

    node->next = before;
    node->prev = before->prev;
    before->prev->next = node;
    before->prev = node;
}

void list_remove(struct Node *node) {
    assert(node);
    assert(node->next && node->next != node);
    assert(node->prev && node->prev != node);

    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->next = NULL;
    node->prev = NULL;
}

static struct Node *node_alloc(void *data) {
    struct Node *node = GC_MALLOC(sizeof *node);
    *node = (struct Node){.data = data};
    return node;
}

void list_push(struct Node *list, void *data) {
    assert(list);
    list_insert(node_alloc(data), list);
}

void list_unshift(struct Node *list, void *data) {
    assert(list);
    list_insert(node_alloc(data), list->next);
}

void *list_pop(struct Node *list) {
    assert(list);
    struct Node *node = list->prev != list ? list->prev : NULL;
    if (node) {
        list_remove(node);
    }
    return node->data;
}

void *list_shift(struct Node *list) {
    assert(list);
    struct Node *node = (list->next != list) ? list->next : NULL;
    if (node) {
        list_remove(node);
    }
    return node->data;
}

struct Node *list_find(const struct Node *list, void *data) {
    for (struct Node *begin = list->next; begin != list; begin = begin->next) {
        if (begin->data == data) {
            return begin;
        }
    }
    return NULL;
}

int list_length(const struct Node *list) {
    int length = 0;
    for (struct Node *begin = list->next; begin != list; begin = begin->next) {
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
