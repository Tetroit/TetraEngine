//
// Created by pchyo on 13/10/2025.
//

#include "UpdateManager.h"

namespace TetraEngine {
    void UpdateManager::AddUpdatable(IUpdatable *updatable) {
        toUpdate.push_back(updatable);
    }

    void UpdateManager::RemoveUpdatable(IUpdatable *updatable) {
        auto loc = std::find(toUpdate.begin(), toUpdate.end(), updatable);
        if (loc != toUpdate.end()) {
            toUpdate.erase(loc);
        }
    }

    void UpdateManager::UpdateAll(float dt) const {
        for (auto& updatable : toUpdate) {
            updatable->Update(dt);
        }
    }

    void UpdateManager::LateUpdateAll(float dt) const {
        for (auto& updatable : toUpdate) {
            updatable->LateUpdate(dt);
        }
    }

    void UpdateManager::Clear() {
        toUpdate.clear();
    }
} // TetraEngine