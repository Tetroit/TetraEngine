#pragma once

#include "../../Core.h"
#include "../../ecs/ECS.h"

using namespace TetraEngine;
namespace Demo2 {
    class PlayerControls {
        ECS::Handle<RigidBody> bodyHandle;
        void ProcessInput(const Event<InputEvent>& ev) {

            auto keyEv = ev.ToType<AnyKeyEvent>();
            if (keyEv.key == GLFW_KEY_E) {
            }
        }
    };
} // Demo2