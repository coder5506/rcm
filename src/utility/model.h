// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <algorithm>
#include <vector>

template<typename T> class Observer {
public:
    virtual ~Observer() = default;
    virtual void on_changed(T& model) = 0;
};

template<typename T> class Model {
    std::vector<Observer<T>*> observers;

public:
    virtual ~Model() = default;

    void observe(Observer<T>* observer) {
        observers.push_back(observer);
    }

    void unobserve(Observer<T>* observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end());
    }

    void changed() {
        for (auto observer : observers) {
            observer->on_changed(*dynamic_cast<T*>(this));
        }
    }
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
