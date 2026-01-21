#pragma once
#include "../../GameObject.h"
#include "../../behaviours/UpdateManager.h"
#include "../../physics/PhysicsCallback.h"

namespace TetraEngine {
    class GameObject;
}

using namespace TetraEngine;
namespace GameDemo {

    class Collectable {
        GameObject* object;
        ECS::Handle<RigidBody> rigidBody;
    public:
        Collectable(GameObject* object, ECS::Handle<RigidBody> rigidBody) : object(object), rigidBody(rigidBody) {}
        virtual ~Collectable() {}
        Action<ECS::Entity, ECS::Entity, TriggerInfo>* GetTriggerAction() {
            TETRA_USE_MAIN_ECS
            return &ecs.GetComponent(rigidBody)->OnTriggerEnter;
        }
        GameObject* GetGameObject() {
            return object;
        }
        ECS::Entity GetEntity() {
            return object->GetEntity();
        }
    };
    class CollectManager : public IUpdatable {
        static CollectManager* instance;
        std::unordered_map<ECS::Entity, Collectable*> persistentLookup;
        std::vector<ECS::Entity> toRemove;
        int score;
        void AddLateRemove(ECS::Entity entity) {
            toRemove.push_back(entity);
        }
    public:
        static void OnTriggerEnter(ECS::Entity trig, ECS::Entity other, TriggerInfo info) {
            TETRA_USE_MAIN_ECS
            LOG("item collected");
            auto manager = instance;
            Collectable* trigCollectable = manager->persistentLookup.at(trig);
            auto gameObject = trigCollectable->GetGameObject();
            manager->AddLateRemove(gameObject->GetEntity());
        }
        CollectManager();
        void Subscribe(Collectable* object);
        void Update(float dt) override;
        void LateUpdate(float dt) override;
        void Unsubscribe(ECS::Entity entity);
        void Unsubscribe(Collectable* entity);
        void Destroy(ECS::Entity entity);
        void Destroy(Collectable* entity);
        void ShowScore(ImVec2 pos, ImVec2 size) const;
    };
} // GameDemo
