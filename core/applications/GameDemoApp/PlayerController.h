#pragma once
#include "../../ecs/ECS.h"

namespace TetraEngine {
    class RigidBody;
}

using namespace TetraEngine;
namespace GameDemo {
    class PlayerController {
        ECS::Handle<RigidBody> rigidBody;

        void Move(glm::vec2 delta, float dt);
    };
} // GameDemo