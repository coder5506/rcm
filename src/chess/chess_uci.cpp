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
#include <ctime>
#include <stdexcept>

#include <unistd.h>

using namespace std;
using namespace thc;

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

void UCIEngine::printf(const char* format, ...) {
    assert(format);

    va_list args;
    va_start(args, format);

    va_list trace_args;
    va_copy(trace_args, args);
    ::printf("uci_printf: ");
    vprintf(format, trace_args);
    va_end(trace_args);

    vdprintf(write_fd, format, args);
    va_end(args);
}

unique_ptr<UCIMessage> UCIEngine::read_request() {
    if (request_queue.empty()) {
        return nullptr;
    }
    auto request = std::move(request_queue.front());
    request_queue.pop();
    return request;
}

UCIMessage* UCIEngine::peek_request() {
    const lock_guard<std::mutex> lock{mutex};
    return request_queue.empty() ? nullptr : request_queue.front().get();
}

void UCIEngine::send_response(unique_ptr<UCIMessage> response) {
    const lock_guard<std::mutex> lock{mutex};
    response_queue.push(std::move(response));
}

bool UCIEngine::handle_request(unique_ptr<UCIMessage> request) {
    const auto result = request->handle_exchange(*this);
    if (result) {
        send_response(std::move(request));
    }
    return result;
}

void UCIEngine::engine_thread() {
    for (;;) {
        unique_ptr<UCIMessage> request;
        {
            unique_lock<std::mutex> lock{mutex};
            request = read_request();
            while (!request) {
                cond.wait(lock);
                request = read_request();
            }
        }
        if (!handle_request(std::move(request))) {
            break;
        }
    }

    buffer.close();

    close(write_fd);
    write_fd = -1;
}

UCIEngine::~UCIEngine() {
    if (write_fd >= 0) {
        quit();
    }
}

UCIEngine::UCIEngine(int read_fd, int write_fd)
    : buffer{8192, read_fd},
      write_fd{write_fd},
      thread{&UCIEngine::engine_thread, this}
{
    assert(read_fd  >= 0);
    assert(write_fd >= 0);
    send(make_unique<UCIMessage>());
}

unique_ptr<UCIEngine> UCIEngine::execvp(const char* file, char *const argv[]) {
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
        return make_unique<UCIEngine>(read_pipe[0], write_pipe[1]);
    }

    dup2(read_pipe[1], STDOUT_FILENO);
    dup2(write_pipe[0], STDIN_FILENO);
    for (int i = 0; i != 4; ++i) {
        close(pipe_fds[i]);
    }

    ::execvp(file, argv);
    _exit(EXIT_FAILURE);

error:
    for (int i = 0; i != 4; ++i) {
        if (pipe_fds[i] >= 0) {
            close(pipe_fds[i]);
        }
    }
    return nullptr;
}

void UCIEngine::send(unique_ptr<UCIMessage> request) {
    const lock_guard<std::mutex> lock{mutex};
    request_queue.push(std::move(request));
    cond.notify_one();
}

unique_ptr<UCIMessage> UCIEngine::receive() {
    const lock_guard<std::mutex> lock{mutex};
    if (response_queue.empty()) {
        return nullptr;
    }
    auto response = std::move(response_queue.front());
    response_queue.pop();
    return response;
}

void UCIEngine::quit() {
    send(make_unique<UCIQuitMessage>());
    thread.join();
}

bool UCIMessage::handle_exchange(UCIEngine& engine) {
    engine.printf("uci\n");
    const auto timeout = time(NULL) + 5;
    while (time(NULL) < timeout) {
        if (auto line = engine.expect("uciok")) {
            goto success;
        }
    }
    return false;

success:
    engine.printf("setoption name Threads value 2\n");
    engine.printf("setoption name Hash value 192\n");
    return true;
}

bool UCIQuitMessage::handle_exchange(UCIEngine& engine) {
    return false;
}

bool UCIPlayMessage::expect_bestmove(UCIEngine& engine) {
    engine.printf("setoption MultiPV 1\n");
    engine.printf("position fen %s\n", game->fen().data());

    char color = game->WhiteToPlay() ? 'w' : 'b';
    engine.printf("go %ctime 60000 %cinc 600\n", color, color);

    for (;;) {
        if (engine.peek_request()) {
            return true;
        }

        if (auto line = engine.expect("bestmove ")) {
            try {
                move = game->uci_move(line + 9);
                return true;
            }
            catch (const logic_error&) {
                return false;
            }
        }
    }
}

bool UCIPlayMessage::handle_exchange(UCIEngine& engine) {
    engine.printf("setoption name UCI_Elo value %d\n", elo);
    engine.printf("setoption name UCI_LimitStrength value true\n");
    return expect_bestmove(engine);
}

bool UCIHintMessage::handle_exchange(UCIEngine& engine) {
    engine.printf("setoption name UCI_LimitStrength value false\n");
    return expect_bestmove(engine);
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
