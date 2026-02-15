#include "ForD.h"
#include "../rendering/Shader.h"
#include "../rendering/Skybox.h"

ForD::ForD() {
    myScene.skybox = new Skybox(Skybox::BOX, assetPath + "/skybox");
    Skybox::current = myScene.skybox;
    litShader = std::make_unique<Shader>(shaderPath + "/lit.glvs", shaderPath + "/lit.glfs");
}