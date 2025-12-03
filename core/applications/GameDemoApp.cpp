//
// Created by pchyo on 03/12/2025.
//

#include "GameDemoApp.h"
#include "../rendering/Scene.h"

GameDemoApp::GameDemoApp() {
}

GameDemoApp::~GameDemoApp() {
    delete ground;
    delete ball;
    delete scene;
}

void GameDemoApp::Update() {
    Application::Update();
}

void GameDemoApp::DrawGUI(ImVec2 origin, ImVec2 size) {
    Application::DrawGUI(origin, size);
}
