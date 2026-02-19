#include "ForD.h"
#include "../rendering/Shader.h"
#include "../rendering/Skybox.h"
#include "../rendering/VertexData.h"
#include "../rendering/4d/Mesh4D.h"


ForD::ForD() {
    myScene.skybox = new Skybox(Skybox::BOX, assetPath + "/skybox");
    Skybox::current = myScene.skybox;
    litShader = std::make_unique<Shader>(
        shaderPath + "/lit.glvs",
        shaderPath + "/lit.glfs");
    geomShader = std::make_unique<Shader>(
        shaderPath + "/geomTest.glvs",
        shaderPath + "/geomTest.glgs",
        shaderPath + "/geomTest.glfs");
    forDShader = std::make_unique<Shader>(
        shaderPath + "/lit4D.glvs",
        shaderPath + "/lit4D.glfs");

    GameObject* go = new GameObject("Harold");

    auto cubeVD = VertexData::GetPrefab(VD_CUBE);
    go->AddComponent<MeshRenderer>(cubeVD, geomShader.get());
    myScene.AddObject(*go);

    mesh4D = std::make_unique<Mesh4D>();
    mesh4D->AddTetrahedron(Tetrahedron4D(
        {-1.0f, 0.5f, 0.5f, -0.5f},
        {-1.2f, 0.5f, -0.5f, -0.7f},
        {1.0f, -0.2f, 0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f, 0.7f}));
    renderer4D = std::make_unique<Renderer4D>(mesh4D.get(), forDShader.get());
}

void ForD::Update() {
    Application::Update();

    renderer4D->Render(ViewProvider::GetCurrent(), glm::mat4(1.0));

}
