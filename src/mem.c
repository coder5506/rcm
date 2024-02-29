// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#include "mem.h"
#include "list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void mem_erase(void *ptr, int size) {
    assert(ptr && size >= 0);
    memset(ptr, 0xA3, size);
}

struct Allocated {
    struct Allocated *next;
    struct Allocated *prev;
    int               size;
    unsigned char     data[];
};

static struct Allocated allocated = LIST_INIT(allocated);

void mem_free(void **ptr, int size) {
    assert(ptr && *ptr);
    assert(size >= 0);

    struct Allocated *a = (struct Allocated*)((unsigned char*)*ptr - sizeof *a);
    assert(a->size == size);
    assert(list_find((struct Node*)&allocated, (struct Node*)a));

    list_remove((struct Node*)a);
    mem_erase(a, sizeof *a + size);
    free(a);
    *ptr = NULL;
}

void mem_alloc(void **ptr, int size) {
    assert(ptr && !*ptr);
    assert(size >= 0);

    struct Allocated *a = malloc(sizeof *a + size);
    if (!a) {
        return;
    }

    mem_erase(a, sizeof *a + size);
    a->next = NULL;
    a->prev = NULL;
    a->size = size;

    // Assuming most frees will be recently allocated
    list_unshift((struct Node*)&allocated, (struct Node*)a);

    *ptr = a->data;
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
