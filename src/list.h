// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

struct Node {
    struct Node *next;
    struct Node *prev;
    void        *data;
};

// Initialize doubly-linked list with sentinel
#define LIST_INIT(list) ((struct Node){.next = &(list), .prev = &(list)})

void list_remove(struct Node *node);

void list_push(struct Node *list, void *data);
void list_unshift(struct Node *list, void *data);
void *list_pop(struct Node *list);
void *list_shift(struct Node *list);

static inline void list_clear(struct Node *list) {
    list->next = list;
    list->prev = list;
}

static inline bool list_empty(const struct Node *list) {
    return list->next == list;
}

struct Node *list_find(const struct Node *list, void *data);
int list_length(const struct Node *list);

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
