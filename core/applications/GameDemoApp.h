#pragma once
#include "../Application.h"
#include "../utils/Brush.h"
#include "GameDemoApp/Ball.h"
#include "GameDemoApp/Ground.h"
#include "GameDemoApp/Level.h"
#include "GameDemoApp/PlayerController.h"

namespace GameDemo {
    class CollectManager;
}

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
    PlayerController* playerController;
    CollectManager* collectManager;
    ECS::Handle<Camera> mainCamera;

    std::unique_ptr<Level> level;

    std::unordered_map<std::string, std::unique_ptr<Material>> materials;
    std::unordered_map<std::string, std::unique_ptr<Texture2D>> textures;
public:
    void AddStar(glm::vec3 pos);

    GameDemoApp();
    ~GameDemoApp() override;
    void Update() override;
    void DrawGUI(ImVec2 origin, ImVec2 size) override;
};
