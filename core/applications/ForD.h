#pragma once
#include "../Application.h"
#include "../Core.h"
#include "../rendering/Scene.h"
#include "../rendering/4d/Renderer4D.h"


using namespace TetraEngine;

class ForD : public Application{
	float timer = 0;
	bool enableAnimation = true;
public:
	Scene myScene = Scene();
    std::unique_ptr<Shader> litShader;
    std::unique_ptr<Shader> geomShader;
	std::unique_ptr<Shader> forDShader;
	std::unique_ptr<Shader> forDWireShader;

	std::unique_ptr<Mesh4D> mesh4D;
	std::shared_ptr<Mesh4D> tesseract;

	std::unique_ptr<Material> copper;
	void Update() override;
	void PauseToggle(const Event<InputEvent>& ev);
    ForD();
};