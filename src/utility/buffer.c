// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "buffer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <sys/select.h>
#include <unistd.h>

#include <gc/gc.h>

struct Buffer {
    char *read;
    char *write;
    char *end;
    int   fd;
    char  begin[];
};

bool buffer_valid(const struct Buffer *buf) {
    return buf &&
        buf->begin <= buf->read  &&
        buf->read  <= buf->write &&
        buf->write <= buf->end   &&
        !*buf->write &&
        buf->fd >= -1;
}

struct Buffer *buffer_new(size_t size, int fd) {
    assert(fd >= 0);

    struct Buffer *buf = GC_MALLOC_ATOMIC(sizeof *buf + size + 1);
    buf->read  = buf->begin;
    buf->write = buf->begin;
    buf->end   = buf->begin + size;
    buf->fd    = fd;
    memset(buf->begin, 0, size + 1);

    assert(buffer_valid(buf));
    return buf;
}

static char *try_getline(struct Buffer *buf) {
    assert(buffer_valid(buf));

    char *line = NULL;
    char *eol  = strchr(buf->read, '\n');
    if (eol) {
        line = buf->read;
        *eol = 0;
        buf->read = eol + 1;
    }

    assert(buffer_valid(buf));
    return line;
}

static void close_buffer(struct Buffer *buf) {
    assert(buffer_valid(buf));
    if (buf->fd >= 0) {
        close(buf->fd);
        buf->fd = -1;
    }
    assert(buffer_valid(buf));
}

static bool can_fill(struct Buffer *buf, long timeout_ms) {
    assert(buffer_valid(buf));
    assert(timeout_ms >= 0);

    if (buf->fd < 0) {
        assert(buf->fd == -1);
        return false;
    }

    const struct timespec timeout = {
        .tv_sec =   timeout_ms / 1000,
        .tv_nsec = (timeout_ms % 1000) * 1000000
    };

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(buf->fd, &read_fds);

    const int rc = pselect(buf->fd + 1, &read_fds, NULL, NULL, &timeout, NULL);
    if (rc < 0 && errno != EINTR) {
        close_buffer(buf);
    }

    assert(buffer_valid(buf));
    return rc == 1;
}

static void try_fill(struct Buffer *buf, long timeout_ms) {
    if (!can_fill(buf, timeout_ms)) {
        return;
    }

    char *const midpoint = buf->begin + (buf->end - buf->begin) / 2;
    if (buf->write >= midpoint) {
        memmove(buf->begin, buf->read, buf->write - buf->read);
        buf->write -= buf->read - buf->begin;
        buf->read   = buf->begin;
        *buf->write = 0;
    }

    ssize_t n_read = read(buf->fd, buf->write, buf->end - buf->write);
    if (n_read < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            n_read = 0;
        }
    }

    if (n_read >= 0) {
        buf->write += n_read;
        *buf->write = 0;
    } else {
        close_buffer(buf);
    }

    assert(buffer_valid(buf));
}

char *buffer_getline(struct Buffer *buf, long timeout_ms) {
    char *const line = try_getline(buf);
    if (line) {
        return line;
    }

    try_fill(buf, timeout_ms);
    return try_getline(buf);
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
