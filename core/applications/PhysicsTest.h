#pragma once

#include "../Application.h"
#include "../rendering/Scene.h"
#include "../rendering/Shader.h"
#include "../rendering/Material.h"
#include "../GameObject_old.h"
#include "../GameObject.h"

using namespace TetraEngine;

class PhysicsTest : public Application {

    Scene myScene = Scene();
    Shader* litShader = nullptr;
    GameObject* cube1 = nullptr;
    GameObject* cube2 = nullptr;
    GameObject* floor = nullptr;
    Material* MaterialC1 = nullptr;
    Material* MaterialC2 = nullptr;
    ECS::Handle<RigidBody> hRigidBodyF;
    ECS::Handle<RigidBody> hRigidBodyC1;
    ECS::Handle<RigidBody> hRigidBodyC2;

    bool simulationStarted = false;
    bool simulationPaused = false;
public:
    PhysicsTest();
    ~PhysicsTest() override;
    void ProcessInput(const Event<InputInfo>& ev);
    void StartSimulation();
    void PauseSimulation();
    void ResumeSimulation();
    void Update() override;
};