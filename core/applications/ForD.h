#pragma once
#include "../Application.h"
#include "../Core.h"
#include "../rendering/Scene.h"

using namespace TetraEngine;

class ForD : public Application{
public:
	Scene myScene = Scene();
    std::unique_ptr<Shader> litShader;
    ForD();
};