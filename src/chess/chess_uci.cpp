// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_uci.h"
#include "chess.h"
#include "../utility/buffer.h"

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <time.h>
#include <unistd.h>

using namespace std;
using namespace thc;

std::unique_ptr<UCIMessage> UCIEngine::receive() {
    const std::lock_guard<std::mutex> lock{mutex};
    if (response_queue.empty()) {
        return nullptr;
    }
    auto response = std::move(response_queue.front());
    response_queue.pop();
    return response;
}

void UCIEngine::send(std::unique_ptr<UCIMessage> request) {
    const std::lock_guard<std::mutex> lock{mutex};
    request_queue.push(std::move(request));
    cond.notify_one();
}

char* UCIEngine::getline() {
    return buffer.getline(1000 /* milliseconds */);
}

char* UCIEngine::expect(const char* startswith) {
    auto line = getline();
    if (line && strncmp(line, startswith, strlen(startswith)) == 0) {
        return line;
    }
    return nullptr;
}

void UCIEngine::uci_printf(const char* format, ...) {
    assert(format);

    va_list args;
    va_start(args, format);

    va_list trace_args;
    va_copy(trace_args, args);
    printf("uci_printf: ");
    vprintf(format, trace_args);
    va_end(trace_args);

    vdprintf(write_fd, format, args);
    va_end(args);
}

std::unique_ptr<UCIMessage> UCIEngine::peek_request() {
    const std::lock_guard<std::mutex> lock{mutex};
    if (request_queue.empty()) {
        return nullptr;
    }
    auto request = std::move(request_queue.front());
    request_queue.pop();
    return request;
}

int UCIMessage::handle_exchange(UCIEngine& engine) {
    engine.uci_printf("uci\n");
    const auto timeout = time(NULL) + 5;
    while (time(NULL) < timeout) {
        const char* line = engine.getline();
        if (line && strcmp(line, "uciok") == 0) {
            goto success;
        }
    }
    return 1;

success:
    engine.uci_printf("setoption name Threads value 2\n");
    engine.uci_printf("setoption name Hash value 192\n");
    return 0;
}

int UCIQuitMessage::handle_exchange(UCIEngine& engine) {
    return 1;
}

int UCIPlayMessage::expect_bestmove(UCIEngine& engine) {
    engine.uci_printf("setoption MultiPV 1\n");
    engine.uci_printf("position fen %s\n", game->fen().data());

    char color = game->WhiteToPlay() ? 'w' : 'b';
    engine.uci_printf("go %ctime 60000 %cinc 600\n", color, color);

    for (;;) {
        if (engine.peek_request()) {
            return 0;
        }

        char *const line = engine.expect("bestmove ");
        if (line) {
            try {
                move = game->uci_move(line + 9);
                return 0;
            }
            catch (const std::logic_error&) {
                return 1;
            }
        }
    }
}

int UCIPlayMessage::handle_exchange(UCIEngine& engine) {
    engine.uci_printf("setoption name UCI_Elo value %d\n", elo);
    engine.uci_printf("setoption name UCI_LimitStrength value true\n");
    return expect_bestmove(engine);
}

int UCIHintMessage::handle_exchange(UCIEngine& engine) {
    engine.uci_printf("setoption name UCI_LimitStrength value false\n");
    return expect_bestmove(engine);
}

void UCIEngine::send_response(std::unique_ptr<UCIMessage> response) {
    const std::lock_guard<std::mutex> lock{mutex};
    response_queue.push(std::move(response));
}

int UCIEngine::handle_request(std::unique_ptr<UCIMessage> request) {
    const auto result = request->handle_exchange(*this);
    if (result == 0) {
        send_response(std::move(request));
    }
    return result;
}

void UCIEngine::engine_thread() {
    for (;;) {
        std::unique_ptr<UCIMessage> request;
        {
            std::unique_lock<std::mutex> lock{mutex};
            request = peek_request();
            while (!request) {
                cond.wait(lock);
                request = peek_request();
            }
        }
        if (handle_request(std::move(request)) != 0) {
            break;
        }
    }

    buffer.close();

    uci_printf("quit\n");
    close(write_fd);
    write_fd = -1;
}

UCIEngine::UCIEngine(int read_fd, int write_fd)
    : buffer{8192, read_fd},
      write_fd{write_fd},
      thread{&UCIEngine::engine_thread, this}
{
    assert(read_fd  >= 0);
    assert(write_fd >= 0);

    send(std::make_unique<UCIMessage>());
}

std::unique_ptr<UCIEngine> UCIEngine::execvp(const char* file, char *const argv[]) {
    assert(file && *file);
    assert(argv);

    pid_t pid = -1;

    int  pipe_fds[] = {-1, -1, -1, -1};
    int *const read_pipe  = pipe_fds + 0;
    int *const write_pipe = pipe_fds + 2;
    if (pipe(read_pipe) != 0 || pipe(write_pipe) != 0) {
        goto error;
    }

    pid = fork();
    if (pid < 0) {
        goto error;
    }

    if (pid > 0) {
        close(read_pipe[1]);
        close(write_pipe[0]);
        return std::make_unique<UCIEngine>(read_pipe[0], write_pipe[1]);
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
    return nullptr;
}

void UCIEngine::quit() {
    send(std::make_unique<UCIQuitMessage>());
    thread.join();
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
