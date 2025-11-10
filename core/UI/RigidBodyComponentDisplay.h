#pragma once
#include "ComponentDisplay.h"

namespace TetraEngine::UI {
    class RigidBodyComponentDisplay : public ComponentDisplay {

    public:
        void Draw(ECS::TypeErasedHandle handle) override;
    };
} // TetraEngine