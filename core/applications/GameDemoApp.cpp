//
// Created by pchyo on 03/12/2025.
//

#include "GameDemoApp.h"

#include "../rendering/Material.h"
#include "../rendering/Scene.h"
#include "../rendering/Skybox.h"
#include "../rendering/Texture2D.h"
#include "../utils/OBJParser.h"
#include "GameDemoApp/CameraConstraint.h"

GameDemoApp::GameDemoApp() {
    TETRA_USE_MAIN_ECS
    scene = new Scene();
    scene->skybox = new Skybox(Skybox::BOX, assetPath + "/skybox");
    litShader = new Shader(shaderPath + "/lit.glvs", shaderPath + "/lit.glfs");
    litShader->Use();
    Ball::SetVD(OBJParser::OBJRead(meshPath + "/sphere.obj"));
    ball = new Ball("Player", litShader,
        glm::vec3(0),
        glm::vec3(0),
        glm::vec3(1));
    scene->AddObject(*ball->GetGameObject());
    ground = new Ground(litShader,
        glm::vec3(0, -5, 0),
        glm::vec3(0),
        glm::vec3(100));
    scene->AddObject(*ground->GetGameObject());

    textures.emplace("FloorColor", std::make_unique<Texture2D>(texturePath + "/tileFloor/color.png"));
    textures.emplace("FloorNormal", std::make_unique<Texture2D>(texturePath + "/tileFloor/normal.png"));
    textures.emplace("FloorRoughness", std::make_unique<Texture2D>(texturePath + "/tileFloor/roughness.png"));
    materials.emplace("Ground", std::make_unique<Material>(
        glm::vec3(0.1f),
        glm::vec3(0.6f),
        glm::vec3(0.7f),
        "Ground"));
    materials.emplace("Ball", std::make_unique<Material>(
        glm::vec3(0.1f, 0.1f, 0.0f),
        glm::vec3(0.6f, 0.6f, 0.0f),
        glm::vec3(0.7f, 0.7f, 0.0f),
        "Ball"));

    ball->GetRenderer()->material = materials["Ball"].get();
    ground->GetRenderer()->material = materials["Ground"].get();
    ground->GetRenderer()->textureDiffuse = textures["FloorColor"].get();
    ground->GetRenderer()->textureSpecular = textures["FloorRoughness"].get();
    ground->GetRenderer()->scaleOffsetDiffuse = glm::vec4(10,10,0,0);
    ground->GetRenderer()->scaleOffsetSpecular = glm::vec4(10,10,0,0);

    camera = new GameObject();
    mainCamera = camera->AddComponent<Camera>();
    scene->gameCamera = ecs.GetComponent(mainCamera);
    scene->AddObject(*camera);

    cameraConstraint = new CameraConstraint(
        camera->GetTransformHandle(),
        ball->GetTransformHandle(),
        glm::vec3(0, -5, 5));
    AddUpdatable(cameraConstraint);

}

GameDemoApp::~GameDemoApp() {
    delete ground;
    delete ball;
    delete scene;
    delete litShader;
    delete camera;
    delete cameraConstraint;
}

void GameDemoApp::Update() {
    Application::Update();
}

void GameDemoApp::DrawGUI(ImVec2 origin, ImVec2 size) {
    Application::DrawGUI(origin, size);
}
