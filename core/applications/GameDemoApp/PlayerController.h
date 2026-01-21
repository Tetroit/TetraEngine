#pragma once
#include "../../InputManager.h"
#include "../../behaviours/UpdateManager.h"
#include "../../ecs/ECS.h"
#include "../../utils/Event.h"

namespace TetraEngine {
    class RigidBody;
    class Transform;
}

using namespace TetraEngine;
namespace GameDemo {
    class PlayerController : public IUpdatable {
        ECS::Handle<RigidBody> rigidBody;
        ECS::Handle<Transform> camera;
        glm::vec2 velocity;
        float acceleration = 1.0f;
        float speedCap = 1000.0f;
        void OnKeyPress(const Event<InputEvent>& ev);
    protected:
        void Update(float dt) override;
    public:
        PlayerController(ECS::Handle<RigidBody> rb);
        ~PlayerController() override;
    };
} // GameDemo