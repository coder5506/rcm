// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_UCI_H
#define CHESS_UCI_H

#include "../thc/thc.h"
#include "../utility/buffer.h"

#include <condition_variable>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

class Game;
class UCIMessage;
class UCIPlayMessage;

class UCIEngine {
private:
    std::mutex              mutex; // Lock request and response queues
    std::condition_variable cond;  // Signal new request
    std::thread             thread;
    std::queue<std::unique_ptr<UCIMessage>> request_queue;
    std::queue<std::unique_ptr<UCIMessage>> response_queue;
    Buffer buffer;
    int    write_fd;

    char* getline();
    char* expect(const char* startswith);
    void uci_printf(const char* format, ...);
    std::unique_ptr<UCIMessage> peek_request();
    void send_response(std::unique_ptr<UCIMessage> response);

    int expect_bestmove(std::unique_ptr<UCIPlayMessage> request);
    int handle_hint(std::unique_ptr<UCIPlayMessage> request);
    int handle_play(std::unique_ptr<UCIPlayMessage> request);

    int handle_uci(std::unique_ptr<UCIMessage> request);
    int handle_request(std::unique_ptr<UCIMessage> request);

    void engine_thread();

public:
    static std::unique_ptr<UCIEngine> execvp(const char* file, char *const argv[]);

    UCIEngine(int read_fd, int write_fd);

    std::unique_ptr<UCIMessage> receive();
    void send(std::unique_ptr<UCIMessage> request);

    void quit();
};

class UCIMessage {
public:
    virtual ~UCIMessage() = default;
};

class UCIQuitMessage : public UCIMessage {};

class UCIPlayMessage : public UCIMessage {
public:
    const Game* game;
    int         elo;
    std::optional<thc::Move> move;

    UCIPlayMessage(const Game* game, int elo) : game{game}, elo{elo} {}
};

class UCIHintMessage : public UCIPlayMessage {};

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
