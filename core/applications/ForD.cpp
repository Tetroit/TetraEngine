#include "ForD.h"
#include "../rendering/Shader.h"
#include "../rendering/Skybox.h"
#include "../rendering/VertexData.h"

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

    GameObject* go = new GameObject("Harold");
    auto cubeVD = VertexData::GetPrefab(VD_CUBE);
    go->AddComponent<MeshRenderer>(cubeVD, geomShader.get());
    myScene.AddObject(*go);
}
