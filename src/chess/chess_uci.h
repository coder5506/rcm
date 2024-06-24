// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_UCI_H
#define CHESS_UCI_H

#include "../thc/thc.h"
#include "../utility/buffer.h"

#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>


class Game;
class UCIMessage;

// Wrap UCI engine running in separate process
class UCIEngine {
private:
    std::mutex              mutex;  // Lock request and response queues
    std::condition_variable cond;   // Signal new request (poll for responses)
    std::thread             thread;
    std::queue<std::unique_ptr<UCIMessage>> request_queue;
    std::queue<std::unique_ptr<UCIMessage>> response_queue;
    Buffer buffer;    // Receive from UCI engine
    int    write_fd;  // Send to UCI engine

    // Only spawn one instance of any given UCI engine
    static std::map<std::pair<std::string, std::vector<std::string>>,  // (file, argv)
                    std::shared_ptr<UCIEngine>> instances;

public:
    // Return (possibly new) instance for (file, argv)
    static std::shared_ptr<UCIEngine> execvp(std::string file, std::vector<std::string> argv);

    // Get response from engine thread, if any.  Does not block.
    std::unique_ptr<UCIMessage> receive();

    // Send request to engine thread
    void send(std::unique_ptr<UCIMessage> request);

    // Stop engine
    void quit();

public:
    // For use by UCIMessage implementations
    char* expect(const char* startswith);
    char* getline();
    void  printf(const char* format, ...);

    // Look to see if there's a new request in the queue
    UCIMessage* peek_request();

public:
    // Necessarily public, but let's pretend they're private
    ~UCIEngine();
    UCIEngine(int read_fd, int write_fd);

private:
    // Read from external UCI process
    void engine_thread();

    // Get next request from queue.  Does not lock.
    std::unique_ptr<UCIMessage> read_request();

    // Do whatever is required to process request.  Return false to stop engine.
    bool handle_request(std::unique_ptr<UCIMessage> request);

    // Enqueue response from external process
    void send_response(std::unique_ptr<UCIMessage> response);
};


// Initialize UCI engine
class UCIMessage {
public:
    virtual ~UCIMessage() = default;
    virtual bool handle_exchange(UCIEngine&);
};


// Stop UCI engine
class UCIQuitMessage : public UCIMessage {
public:
    bool handle_exchange(UCIEngine& engine) override;
};


// Request engine to select a move
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


// Request engine to select a move, without limiting strength
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
