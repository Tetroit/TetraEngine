//
// Created by pchyo on 19/12/2025.
//

#include "CollectManager.h"

namespace GameDemo {

    CollectManager* CollectManager::instance = nullptr;

    CollectManager::CollectManager() {
        if (instance == nullptr) {
            instance = this;
        }
        score = 0;
    }

    void CollectManager::Subscribe(Collectable *object) {
        persistentLookup.try_emplace(object->GetEntity(), object);
        object->GetTriggerAction()->AddCallback(OnTriggerEnter, "CollectManager");
    }

    void CollectManager::Update(float dt) {
    }

    void CollectManager::LateUpdate(float dt) {
        for (auto rem : toRemove) {
            Destroy(ECS::Entity(rem));
        }
        toRemove.clear();
    }

    void CollectManager::Unsubscribe(ECS::Entity entity) {
        Collectable *object = persistentLookup.at(entity);
        object->GetTriggerAction()->RemoveCallback("CollectManager");
        auto removeIt = persistentLookup.find(entity);
        if (removeIt != persistentLookup.end()) {
            persistentLookup.erase(removeIt);
        }
    }
    void CollectManager::Unsubscribe(Collectable *object) {
        Unsubscribe(object->GetEntity());
    }
    void CollectManager::Destroy(ECS::Entity entity) {
        Collectable *object = persistentLookup.at(entity);
        Unsubscribe(entity);
        object->GetGameObject()->Destroy();
        delete object;

        score++;
    }
    void CollectManager::Destroy(Collectable *object) {
        Destroy(object->GetEntity());
    }

    void CollectManager::ShowScore(ImVec2 pos, ImVec2 size) const {
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Text("Score: %d", score);
    }
} // GameDemo