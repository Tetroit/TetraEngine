#pragma once
#include "../Application.h"
#include "GameDemoApp/Ball.h"
#include "GameDemoApp/Ground.h"

namespace GameDemo {
    class CameraConstraint;
}

using namespace TetraEngine;
using namespace GameDemo;

namespace TetraEngine {
    class Scene;
}

class GameDemoApp : public Application {
    Scene* scene;
    Shader* litShader;
    Ball* ball;
    Ground* ground;
    GameObject* camera;
    CameraConstraint* cameraConstraint;
    ECS::Handle<Camera> mainCamera;

    std::unordered_map<std::string, std::unique_ptr<Material>> materials;
    std::unordered_map<std::string, std::unique_ptr<Texture2D>> textures;
public:
    GameDemoApp();
    ~GameDemoApp() override;
    void Update() override;
    void DrawGUI(ImVec2 origin, ImVec2 size) override;
};
