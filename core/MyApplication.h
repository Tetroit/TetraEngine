#pragma once
#include "Application.h"
#include "rendering/Scene.h"
#include "rendering/Shader.h"
#include "rendering/MeshRenderer.h"
#include "rendering/PointLight.h"
#include "rendering/Texture2D.h"
#include "rendering/Material.h"
#include "GameObject.h"

namespace TetraEngine {
	class MyApplication : public Application
	{
	public:

		Scene myScene = Scene();
		Shader* litShader;

		PointLight* light1 = new PointLight(glm::vec3(0, 0, 5), "l1");
		PointLight* light2 = new PointLight(glm::vec3(0, 0, -5), "l2");

		MeshRenderer* shipRenderer;
		Material shipMaterial = Material(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), "shipMat");

		GameObject* player;

		Texture2D specularTex;
		Texture2D diffuseTex;
		Texture2D ambientTex;


		MyApplication();
		~MyApplication() override;
		void Update() override;
	};
}

