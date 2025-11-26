#include "tetrapc.h"
#include "Demo2.h"
#include "../Core.h"
#include "../GameObject.h"
#include "../physics/Collider.h"
#include "../rendering/Skybox.h"
#include "../utils/OBJParser.h"
#include "../rendering/VertexData.h"
#include "../utils/Time.h"

using namespace TetraEngine;

Demo2::Demo2() {

    ToggleCursor(false);

    TETRA_USE_MAIN_ECS
    myScene.skybox = new Skybox(Skybox::BOX, assetPath + "/skybox");
    Skybox::current = myScene.skybox;
    litShader = new Shader(shaderPath + "/lit.glvs", shaderPath + "/lit.glfs");
    litShader->Use();

    MtBall = new Material(
        glm::vec3(0.027f, 0.014f, 0.103f),
        glm::vec3(0.142f, 0.655f, 0.258f),
        glm::vec3(0.740f, 0.968f, 0.696f));

    MtDefault = new Material(
        glm::vec3(0.1f),
        glm::vec3(0.7f),
        glm::vec3(0.5f));

    //ball
    ball = new GameObject("Ball");
    myScene.AddObject(*ball);

    ball->GetTransform()->SetLocalScale(glm::vec3(0.5f));
    int MSphereID = OBJParser::OBJRead(meshPath + "/sphere.obj");
    std::shared_ptr<VertexData> VDSphere = VertexData::GetPrefab(MSphereID);

    // auto HMRBall = ball->AddComponent<MeshRenderer>(VDSphere, litShader);
    // auto MRBall = ecs.GetComponent(HMRBall);
    // MRBall->material = MtBall;

    auto HCamBall = ball->AddComponent<Camera>();
    auto CamBall = ecs.GetComponent(HCamBall);
    myScene.gameCamera = CamBall;

    auto HRBBall = ball->AddComponent<RigidBody>();
    auto RBBall = ecs.GetComponent(HRBBall);
    auto* CollBall = new Collider();
    CollBall->AddSphere(ball->GetTransform()->GetLocalScale().x);
    RBBall->SetCollider(CollBall);
    RBBall->AddAngular(glm::vec3(0.1f, 0.2f, 0.3f));

    //ground
    ground = new GameObject("Ground");
    myScene.AddObject(*ground);
    ground->GetTransform()->SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));
    ground->GetTransform()->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
    auto HMRGround = ground->AddComponent<MeshRenderer>(VertexData::GetPrefab(VD_RECTANGLE_Y), litShader);
    auto MRGround = ecs.GetComponent(HMRGround);
    MRGround->material = MtDefault;
    ground->AddComponent<RigidBody>(true, true);

    //light1
    light1 = new GameObject("Light");
    myScene.AddObject(*light1);
    light1->GetTransform()->SetPosition(glm::vec3(-1.0f, 0.0f, 2.0f));
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	auto HPLLight1 = light1->AddComponent<PointLight>(
	    lightColor * 0.1f,
	    lightColor * 0.7f,
	    lightColor * 0.6f,
	    glm::vec3(1.0f, 0.1f, 0.01f)
	);
}

void Demo2::Update() {
}

void Demo2::DrawGUI(ImVec2 origin, ImVec2 size) {

    ImGui::SetCursorPos(ImVec2(origin.x, origin.y));
    if (ImGui::Button("Test", ImVec2(size.x/2, size.y/2))) {
    }
}
