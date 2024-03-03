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
#define LIST_INIT(list) {.next = &(list), .prev = &(list)}

void list_insert(struct Node *node, struct Node *before);
void list_remove(struct Node *node);

static inline void
list_push(struct Node *list, struct Node *node) { list_insert(node, list); }

static inline void list_unshift(struct Node *list, struct Node *node) {
    list_insert(node, list->next);
}

struct Node *list_pop(struct Node *list);
struct Node *list_shift(struct Node *list);

static inline void
list_clear(struct Node *list) { list->next = list; list->prev = list; }

static inline bool
list_empty(const struct Node *list) { return list->next == list; }

struct Node *list_find(const struct Node *list, const struct Node *node);
int list_length(const struct Node *list);

static inline void
list_free(struct Node *list, void (*free_node)(struct Node *node)) {
    (void)free_node;
    list_clear(list);
}

//
// Convenience macros
//

#define LIST_INSERT(node, before) list_insert((struct Node*)(node), (struct Node*)(before))
#define LIST_REMOVE(node) list_remove((struct Node*)(node))
#define LIST_PUSH(list, node) list_push((struct Node*)(list), (struct Node*)(node))
#define LIST_UNSHIFT(list, node) list_unshift((struct Node*)(list), (struct Node*)(node))
#define LIST_POP(list) (struct Node*)list_pop((struct Node*)(list))
#define LIST_SHIFT(list) (struct Node*)list_shift((struct Node*)(list))
#define LIST_CLEAR(list) list_clear((struct Node*)(list))
#define LIST_EMPTY(list) list_empty((const struct Node*)(list))
#define LIST_FIND(list, node) (struct Node*)list_find((const struct Node*)(list), (const struct Node*)(node))
#define LIST_LENGTH(list) list_length((const struct Node*)(list))

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
