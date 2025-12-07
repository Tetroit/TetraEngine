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
#include "PhysicsDemo2/Ball.h"
#include "PhysicsDemo2/Ball.h"


class Ball;
namespace TetraEngine {
    class PhysicsDemo2 : public Application {
        
        Scene myScene = Scene();
        Shader* litShader;
        std::vector<Ball*> balls;
        std::vector<Material*> ballsMaterials;

        GameObject* ball;
        GameObject* ground;
        GameObject* light1;

        Material* MtBall;
        Material* MtDefault;
        int MSphereID;

    public:

        PhysicsDemo2();
        ~PhysicsDemo2();
        void ProcessInput(const Event<GLFWManager::InputEvent> & event);
        void Update() override;
        void DrawGUI(ImVec2 origin, ImVec2 size) override;

        Ball* SpawnBall(std::string&& name, glm::vec3 position, Material* material);
    };
}