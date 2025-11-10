#pragma once
#include "ComponentDisplay.h"


namespace TetraEngine::UI {
    class PointLightComponentDisplay : public  ComponentDisplay {

        static float AttenuationGraph(void* data, int i);
    public:
        void Draw(ECS::TypeErasedHandle handle) override;
    };
} // UI