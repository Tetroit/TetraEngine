#pragma once
#include "ComponentDisplay.h"

namespace TetraEngine::UI {

    struct TransformComponentDisplayPrefs {
        TransformComponentDisplayPrefs() = default;
        bool showLocal = false;
    };

    class TransformComponentDisplay : public ComponentDisplay {

        TransformComponentDisplayPrefs displayPrefs;
        void Draw(ECS::TypeErasedHandle handle) override;
    };
}
