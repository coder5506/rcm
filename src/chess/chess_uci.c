// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_uci.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/select.h>
#include <time.h>

#include <gc/gc.h>
#include <pthread.h>

struct UCIEngine {
    pthread_t thread;
    bool shutdown;
    int  read_fd;
    int  write_fd;
};

int uci_printf(struct UCIEngine *engine, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int n = vdprintf(engine->write_fd, format, args);
    va_end(args);
    return n;
}

static void *engine_thread(void *arg)
{
    struct UCIEngine *engine = arg;
    FILE *in = fdopen(engine->read_fd, "r");

    time_t start = time(NULL);
    bool   uciok = false;
    uci_printf(engine, "uci\n");

    const struct timespec timeout = {.tv_sec = 1};
    fd_set read_fds;

    while (!engine->shutdown) {
        if (!uciok && time(NULL) > start + 5) {
            break;
        }

        FD_ZERO(&read_fds);
        FD_SET(engine->read_fd, &read_fds);
        if (pselect(engine->read_fd + 1, &read_fds, NULL, NULL, &timeout, NULL) == 0) {
            continue;
        }

        char  *line = NULL;
        size_t size = 0;
        const ssize_t n_read = getline(&line, &size, in);
        if (n_read < 0) {
            goto next;
        }

        uciok = uciok || strncmp(line, "uciok", 5) == 0;

    next:
        if (line) {
            free(line);
        }
    }

    fclose(in);
    engine->read_fd = -1;

    uci_printf(engine, "quit\n");
    close(engine->write_fd);
    engine->write_fd = -1;

    return NULL;
}

// static void stop_thread(struct UCIEngine *engine)
// {
//     engine->shutdown = true;
//     pthread_join(engine->thread, NULL);
// }

static struct UCIEngine *uci_new(int read_fd, int write_fd)
{
    struct UCIEngine *engine = GC_MALLOC(sizeof *engine);
    engine->shutdown = false;
    engine->read_fd  = read_fd;
    engine->write_fd = write_fd;
    pthread_create(&engine->thread, NULL, engine_thread, engine);
    return engine;
}

struct UCIEngine *uci_execvp(const char *file, char *const argv[])
{
    int  pipe_fds[] = {-1, -1, -1, -1};
    int *read_pipe  = pipe_fds + 0;
    int *write_pipe = pipe_fds + 2;
    if (pipe(read_pipe) != 0 || pipe(write_pipe) != 0) {
        goto error;
    }

    const pid_t pid = fork();
    if (pid < 0) {
        goto error;
    }

    if (pid > 0) {
        close(read_pipe[1]);
        close(write_pipe[0]);
        return uci_new(read_pipe[0], write_pipe[1]);
    }

    dup2(read_pipe[1], STDOUT_FILENO);
    dup2(write_pipe[0], STDIN_FILENO);
    for (int i = 0; i != 4; ++i) {
        close(pipe_fds[i]);
    }

    execvp(file, argv);
    _exit(1);

error:
    for (int i = 0; i != 4; ++i) {
        if (pipe_fds[i] >= 0) {
            close(pipe_fds[i]);
        }
    }
    return NULL;
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
