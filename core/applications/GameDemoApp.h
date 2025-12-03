#pragma once
#include "../Application.h"
#include "gameDemo/Ball.h"
#include "gameDemo/Ground.h"

using namespace TetraEngine;
using namespace GameDemo;

namespace TetraEngine {
    class Scene;
}

class GameDemoApp : Application {
    Scene* scene;
    Ball* ball;
    Ground* ground;
public:
    GameDemoApp();
    ~GameDemoApp() override;
    void Update() override;
    void DrawGUI(ImVec2 origin, ImVec2 size) override;
};
