#pragma once

#include "tetrapc.h"
#include "../Application.h"
#include "../rendering/Scene.h"
#include "../rendering/Shader.h"
#include "../rendering/MeshRenderer.h"
#include "../rendering/PointLight.h"
#include "../rendering/Texture2D.h"
#include "../rendering/Material.h"
#include "../GameObject.h"


namespace TetraEngine {
    class Demo2 : public Application {
        Scene myScene = Scene();
        Shader* litShader;

        GameObject* ball;
        GameObject* ground;
        GameObject* light1;

        Material* MtBall;
        Material* MtDefault;

    public:
        Demo2();
        void Update() override;
        void DrawGUI(ImVec2 origin, ImVec2 size) override;
    };
}