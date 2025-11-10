#pragma once

#include "../ecs/ECS.h"

namespace TetraEngine::UI {
    class ComponentDisplay {
    public:
        virtual ~ComponentDisplay() = default;
        ComponentDisplay() = default;
        virtual void Draw(ECS::TypeErasedHandle handle) = 0;
    };
}