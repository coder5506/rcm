// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

struct List {
    struct List *next;
    struct List *prev;
    void        *data;
};

static inline struct List*
list_begin(struct List *list) { return list->next; }

static inline struct List*
list_end(struct List *list) { return list; }

static inline void
list_clear(struct List *list) { list->next = list; list->prev = list; }

static inline bool
list_empty(const struct List *list) { return list->next == list; }

static inline bool
list_valid(const struct List *list) { return list && list->data == list; }

struct List *list_new();

void list_link(struct List *node, struct List *before);
void list_unlink(struct List *node);

void list_push(struct List *list, void *data);
void *list_pop(struct List *list);

void list_unshift(struct List *list, void *data);
void *list_shift(struct List *list);

struct List *list_find(const struct List *list, void *data);
void *list_first(const struct List *list);
void *list_index(const struct List *list, int index);
void *list_last(const struct List *list);
int list_length(const struct List *list);

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
