// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef>
#include <string>

class Buffer {
private:
    std::string data;
    char *begin;
    char *read;
    char *write;
    char *end;
    int   fd;

    bool  invariant() const;
    char* try_getline();
    bool  can_fill(long timeout_ms);
    void  try_fill(long timeout_ms);

public:
    Buffer(std::size_t size, int fd);
    ~Buffer();

    void close();
    char* getline(long timeout_ms);
};

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
