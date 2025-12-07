//
// Created by pchyo on 27/10/2025.
//

#include "tetrapc.h"
#include "PhysicsTest.h"
#include "../Core.h"
#include "../physics/Collider.h"
#include "../physics/PhysicsScene.h"
#include "../rendering/Skybox.h"
#include "../utils/OBJParser.h"
#include "../rendering/VertexData.h"
#include "../utils/Time.h"

using namespace TetraEngine;

PhysicsTest::PhysicsTest() {
    TETRA_USE_MAIN_ECS
    TETRA_USE_MAIN_PHYSICS_INSTANCE
    TETRA_USE_GAME_INPUT

    input->AddListener<PhysicsTest>(InputInfo(GLFW_PRESS, TETRA_INPUT_KEY_MODE), &PhysicsTest::ProcessInput, *this);
    myScene.GetPhysicsScene()->SetGravity(glm::vec3(0, -5.0f, 0));
    myScene.skybox = new Skybox(Skybox::BOX, assetPath + "/skybox");
    Skybox::current = myScene.skybox;
    litShader = new Shader(shaderPath + "/lit.glvs", shaderPath + "/lit.glfs");
    litShader->Use();

    MaterialC1 = new Material(
        glm::vec3(0.027f, 0.014f, 0.103f),
        glm::vec3(0.142f, 0.655f, 0.258f),
        glm::vec3(0.740f, 0.968f, 0.696f));
    MaterialC2 = new Material(
        glm::vec3(0.027f, 0.014f, 0.103f),
        glm::vec3(0.8f, 0.6f, 0.6f),
        glm::vec3(0.9f, 0.8f, 0.8f));

    //cube 1
    cube1 = new GameObject("Cube 1");
    myScene.AddObject(*cube1);

    cube1->GetTransform()->LocalTranslate(glm::vec3(5,0,-5));
    auto hMeshRendererC1 = cube1->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_CUBE), litShader);

    auto* meshRenderer = ecs.GetComponent(hMeshRendererC1);
    meshRenderer->material = MaterialC1;

    //cube 2
    cube2 = new GameObject("Cube 2");
    myScene.AddObject(*cube2);

    cube2->GetTransform()->LocalTranslate(glm::vec3(5,5,5));
    auto hMeshRendererC2 = cube2->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_CUBE), litShader);

    meshRenderer = ecs.GetComponent(hMeshRendererC2);
    meshRenderer->material = MaterialC2;

    floor = new GameObject("Floor");
    myScene.AddObject(*floor);

    floor->GetTransform()->SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));
    floor->GetTransform()->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
    auto hMeshRendererF = floor->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_RECTANGLE_Y), litShader);
    meshRenderer = ecs.GetComponent(hMeshRendererF);
    meshRenderer->material = MaterialC2;

    hRigidBodyF = floor->AddComponent<RigidBody>(true, true);
}

PhysicsTest::~PhysicsTest() {
    myScene.Clear();
}

void PhysicsTest::ProcessInput(const Event<InputInfo>& ev) {

    auto keyEv = ev.ToType<AnyKeyEvent>();
    if (keyEv.key == GLFW_KEY_E) {
        if (!simulationStarted)
            StartSimulation();
        else {
            if (!simulationPaused)
                PauseSimulation();
            else
                ResumeSimulation();
        }
    }
}

void PhysicsTest::StartSimulation() {
    if (simulationStarted) {
        return;
    }
    TETRA_USE_MAIN_ECS

    hRigidBodyC1 = cube1->AddComponent<RigidBody>();
    auto* rigidBody = ecs.GetComponent(hRigidBodyC1);
    auto* collider = new Collider();
    collider->AddBox(1);
    rigidBody->SetCollider(collider);
    rigidBody->AddAngular(glm::vec3(0.25f,0.63f,1.87f));
    rigidBody->AddLinear(glm::vec3(0.0f, 1.0f, 1.0f));

    hRigidBodyC2 = cube2->AddComponent<RigidBody>();
    rigidBody = ecs.GetComponent(hRigidBodyC2);
    rigidBody->SetCollider(collider);
    rigidBody->AddAngular(glm::vec3(1,1,1));
    rigidBody->AddLinear(glm::vec3(0.0f, 1.0f, -2.0f));

    simulationStarted = true;
    simulationPaused = false;
}

void PhysicsTest::PauseSimulation() {

    TETRA_USE_MAIN_ECS
    ecs.GetComponent(hRigidBodyC1)->SetStatic();
    ecs.GetComponent(hRigidBodyC2)->SetStatic();
    simulationPaused = true;
}

void PhysicsTest::ResumeSimulation() {
    TETRA_USE_MAIN_ECS
    ecs.GetComponent(hRigidBodyC1)->SetDynamic();
    ecs.GetComponent(hRigidBodyC2)->SetDynamic();
    simulationPaused = false;
}

void PhysicsTest::Update() {
    TETRA_USE_MAIN_ECS
    if (simulationPaused && !simulationPaused) {
    }
}
