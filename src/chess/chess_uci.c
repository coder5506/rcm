// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_uci.h"
#include "chess.h"
#include "../utility/list.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/select.h>
#include <time.h>

#include <gc/gc.h>
#include <pthread.h>

struct UCIEngine {
    pthread_mutex_t mutex;  // Lock request and response queues
    pthread_cond_t  cond;   // Signal new request
    pthread_t       thread;
    struct List    *request_queue;
    struct List    *response_queue;
    int             read_fd;
    int             write_fd;
    FILE           *in;
    FILE           *out;
    int             elo;
};

struct UCIMessage *uci_receive(struct UCIEngine *engine) {
    assert(engine);

    pthread_mutex_lock(&engine->mutex);
    struct UCIMessage *response = list_shift(engine->response_queue);
    pthread_mutex_unlock(&engine->mutex);
    return response;
}

void uci_send(struct UCIEngine *engine, struct UCIMessage *request) {
    assert(engine);
    assert(request);

    pthread_mutex_lock(&engine->mutex);
    list_push(engine->request_queue, request);
    pthread_cond_signal(&engine->cond);
    pthread_mutex_unlock(&engine->mutex);
}

static char *uci_getline(struct UCIEngine *engine) {
    const struct timespec timeout = {.tv_sec = 1};
    fd_set read_fds;

    FD_ZERO(&read_fds);
    FD_SET(engine->read_fd, &read_fds);
    if (pselect(engine->read_fd + 1, &read_fds, NULL, NULL, &timeout, NULL) == 0) {
        return NULL;
    }

    char  *line = NULL;
    size_t size = 0;
    const ssize_t n_read = getline(&line, &size, engine->in);

    char *result = NULL;
    if (n_read > 0) {
        result = GC_MALLOC_ATOMIC(n_read + 1);
        memcpy(result, line, n_read);
        result[n_read] = '\0';
    }

    if (line) {
        free(line);
    }

    return result;
}

static int uci_printf(struct UCIEngine *engine, const char *format, ...) {
    va_list args;
    va_start(args, format);
    const int n = vfprintf(engine->out, format, args);
    va_end(args);
    return n;
}

static struct UCIMessage *peek_request(struct UCIEngine *engine) {
    pthread_mutex_lock(&engine->mutex);
    struct UCIMessage *request = list_first(engine->request_queue);
    pthread_mutex_unlock(&engine->mutex);
    return request;
}

static void send_response(struct UCIEngine *engine, struct UCIMessage *response) {
    pthread_mutex_lock(&engine->mutex);
    list_push(engine->response_queue, response);
    pthread_mutex_unlock(&engine->mutex);
}

static int expect_bestmove(struct UCIEngine *engine, struct UCIPlayMessage *request) {
    uci_printf(engine, "setoption MultiPV 1\n");
    uci_printf(engine, "position fen %s\n", game_fen(request->game));

    char color = game_current((struct Game*)request->game)->turn;
    uci_printf(engine, "go %ctime 60000 %cinc 600\n", color, color);

    while (1) {
        if (peek_request(engine)) {
            return 0;
        }

        char *line = uci_getline(engine);
        if (line && strncmp(line, "bestmove ", 9) == 0) {
            request->move = move_from_name(line + 9);
            send_response(engine, (struct UCIMessage*)request);
            return 0;
        }
    }
}

static int handle_hint(struct UCIEngine *engine, struct UCIPlayMessage *request) {
    uci_printf(engine, "setoption name UCI_LimitStrength value false\n");
    return expect_bestmove(engine, request);
}

static int handle_play(struct UCIEngine *engine, struct UCIPlayMessage *request) {
    uci_printf(engine, "setoption name UCI_Elo value %d\n", engine->elo);
    uci_printf(engine, "setoption name UCI_LimitStrength value true\n");
    return expect_bestmove(engine, request);
}

static int handle_uci(struct UCIEngine *engine, struct UCIMessage *request) {
    (void)request;

    uci_printf(engine, "uci\n");
    const time_t timeout = time(NULL) + 5;
    while (time(NULL) < timeout) {
        const char *line = uci_getline(engine);
        if (line && strcmp(line, "uciok\n") == 0) {
            goto success;
        }
    }
    return 1;

success:
    uci_printf(engine, "setoption name Threads value 2\n");
    uci_printf(engine, "setoption name Hash value 192\n");
    return 0;
}

static int handle_request(struct UCIEngine *engine, struct UCIMessage *request) {
    switch (request->type) {
    case UCI_REQUEST_HINT:
        return handle_hint(engine, (struct UCIPlayMessage*)request);
    case UCI_REQUEST_PLAY:
        return handle_play(engine, (struct UCIPlayMessage*)request);
    case UCI_REQUEST_UCI:
        return handle_uci(engine, request);
    case UCI_REQUEST_QUIT:
    default:
        return 1;
    }
}

static void *engine_thread(void *arg) {
    struct UCIEngine *const engine = arg;

    while (1) {
        pthread_mutex_lock(&engine->mutex);
        struct UCIMessage *request = list_shift(engine->request_queue);
        while (!request) {
            pthread_cond_wait(&engine->cond, &engine->mutex);
            request = list_shift(engine->request_queue);
        }
        pthread_mutex_unlock(&engine->mutex);

        if (handle_request(engine, request) != 0) {
            break;
        }
    }

    fclose(engine->in);
    engine->read_fd = -1;

    uci_printf(engine, "quit\n");
    fclose(engine->out);
    engine->write_fd = -1;

    return NULL;
}

static struct UCIEngine *uci_new(int read_fd, int write_fd) {
    struct UCIEngine *engine = GC_MALLOC(sizeof *engine);

    engine->request_queue  = list_new();
    engine->response_queue = list_new();
    engine->read_fd  = read_fd;
    engine->write_fd = write_fd;
    engine->in  = fdopen(engine->read_fd, "r");
    engine->out = fdopen(engine->write_fd, "w");
    engine->elo = 2000;

    pthread_mutex_init(&engine->mutex, NULL);
    pthread_cond_init(&engine->cond, NULL);
    pthread_create(&engine->thread, NULL, engine_thread, engine);

    struct UCIMessage *uci = GC_MALLOC_ATOMIC(sizeof *uci);
    uci->type = UCI_REQUEST_UCI;
    uci_send(engine, uci);

    return engine;
}

struct UCIEngine *uci_execvp(const char *file, char *const argv[]) {
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
    _exit(EXIT_FAILURE);

error:
    for (int i = 0; i != 4; ++i) {
        if (pipe_fds[i] >= 0) {
            close(pipe_fds[i]);
        }
    }
    return NULL;
}

void uci_quit(struct UCIEngine *engine) {
    struct UCIMessage quit = {.type = UCI_REQUEST_QUIT};
    uci_send(engine, &quit);

    pthread_join(engine->thread, NULL);
    pthread_mutex_destroy(&engine->mutex);
    pthread_cond_destroy(&engine->cond);
}

void uci_set_elo(struct UCIEngine *engine, int elo) {
    if (elo < 1400 || 3000 < elo) {
        elo = 2000;
    }
    engine->elo = elo;
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
