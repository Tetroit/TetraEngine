#pragma once
#include "ComponentDisplay.h"

namespace TetraEngine::UI {
    struct GameObjectInfoComponentDisplayPrefs {
    };

    class GameObjectInfoComponentDisplay : public ComponentDisplay {
        void Draw(ECS::TypeErasedHandle handle) override;
    };
}
