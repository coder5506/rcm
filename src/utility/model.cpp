// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "model.h"

#include <algorithm>

bool operator==(const Observer& a, const Observer& b) {
    return a.model_changed == b.model_changed && a.data == b.data;
}

void Model::observe(ModelChanged model_changed, void* data) {
    observers.push_back({model_changed, data});
}

void Model::unobserve(ModelChanged model_changed, void* data) {
    Observer unobserve{model_changed, data};
    std::remove(observers.begin(), observers.end(), unobserve);
}

void Model::changed() {
    for (auto observer : observers) {
        observer.model_changed(this, observer.data);
    }
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
