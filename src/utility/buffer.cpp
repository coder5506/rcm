// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "buffer.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <errno.h>
#include <sys/select.h>
#include <unistd.h>

bool Buffer::invariant() const {
    return (
        begin  <= read  &&
        read   <= write &&
        write  <= end   &&
        *write == '\0'  &&
        fd     >= -1
    );
}

Buffer::Buffer(std::size_t size, int fd) : fd{fd} {
    data.resize(size + 1);
    begin = data.data();
    read  = begin;
    write = begin;
    end   = begin + size;
    assert(invariant());
}

void Buffer::close() {
    assert(invariant());
    if (fd >= 0) {
        ::close(fd);
        fd = -1;
    }
    assert(invariant());
}

Buffer::~Buffer() {
    close();
}

char* Buffer::try_getline() {
    assert(invariant());

    char* line = NULL;
    if (char *const eol = strchr(read, '\n')) {
        line = read;
        *eol = 0;
        read = eol + 1;
    }

    assert(invariant());
    return line;
}

bool Buffer::can_fill(long timeout_ms) {
    assert(invariant());
    assert(timeout_ms >= 0);

    if (fd < 0) {
        assert(fd == -1);
        return false;
    }

    const struct timespec timeout = {
        .tv_sec  =  timeout_ms / 1000,
        .tv_nsec = (timeout_ms % 1000) * 1000000
    };

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    const int rc = pselect(fd + 1, &read_fds, NULL, NULL, &timeout, NULL);
    if (rc < 0 && errno != EINTR) {
        close();
    }

    assert(invariant());
    return rc == 1;
}

void Buffer::try_fill(long timeout_ms) {
    if (!can_fill(timeout_ms)) {
        return;
    }

    char *const midpoint = begin + (end - begin) / 2;
    if (write >= midpoint) {
        memmove(begin, read, write - read);
        write -= read - begin;
        read   = begin;
        *write = '\0';
    }

    ssize_t n_read = ::read(fd, write, end - write);
    if (n_read < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            n_read = 0;
        }
    }

    if (n_read >= 0) {
        write += n_read;
        *write = '\0';
    } else {
        close();
    }

    assert(invariant());
}

char* Buffer::getline(long timeout_ms) {
    if (char* line = try_getline()) {
        return line;
    }

    try_fill(timeout_ms);
    return try_getline();
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
