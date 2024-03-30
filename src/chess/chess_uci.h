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

class UCIEngine {
    std::mutex              mutex; // Lock request and response queues
    std::condition_variable cond;  // Signal new request
    std::thread             thread;
    std::queue<std::unique_ptr<UCIMessage>> request_queue;
    std::queue<std::unique_ptr<UCIMessage>> response_queue;
    Buffer buffer;
    int    write_fd;

public:
    char* expect(const char* startswith);
    char* getline();
    void  printf(const char* format, ...);

    std::unique_ptr<UCIMessage> read_request();
    UCIMessage* peek_request();
    void send_response(std::unique_ptr<UCIMessage> response);
    bool handle_request(std::unique_ptr<UCIMessage> request);

    void engine_thread();

public:
    static std::unique_ptr<UCIEngine> execvp(const char* file, char *const argv[]);

    UCIEngine(int read_fd, int write_fd);

    void send(std::unique_ptr<UCIMessage> request);
    std::unique_ptr<UCIMessage> receive();

    void quit();
};

class UCIMessage {
public:
    virtual ~UCIMessage() = default;
    virtual bool handle_exchange(UCIEngine&);
};

class UCIQuitMessage : public UCIMessage {
public:
    bool handle_exchange(UCIEngine& engine) override;
};

class UCIPlayMessage : public UCIMessage {
public:
    const Game* game;
    int         elo;
    std::optional<thc::Move> move;

    UCIPlayMessage(const Game* game, int elo) : game{game}, elo{elo} {}
    bool handle_exchange(UCIEngine& engine) override;

protected:
    bool expect_bestmove(UCIEngine& engine);
};

class UCIHintMessage : public UCIPlayMessage {
public:
    bool handle_exchange(UCIEngine& engine) override;
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
