#pragma once
#define TETRA_DEBUG_UI true


#include "../Application.h"
#include "../rendering/Scene.h"
#include "../rendering/Shader.h"
#include "../rendering/MeshRenderer.h"
#include "../rendering/PointLight.h"
#include "../rendering/Texture2D.h"
#include "../rendering/Material.h"
#include "../GameObject_old.h"
#include "../GameObject.h"


namespace TetraEngine {
	class MyApplication : public Application
	{
	public:

		Scene myScene = Scene();
		Shader* litShader;

		//PointLight* light1 = new PointLight(glm::vec3(0, 0, 5), "l1");
		//PointLight* light2 = new PointLight(glm::vec3(0, 0, -5), "l2");

		ECS::Handle<MeshRenderer> shipRenderer;
		Material shipMaterial = Material(glm::vec3(0, 0, 0), glm::vec3(0.3, 0.5, 1.0), glm::vec3(0.7, 0.8, 1.0), "shipMat");

		GameObject* shipObject;
		GameObject* light;
		GameObject* light2;

		GameObject* cube1;
		GameObject* cube2;
		GameObject* cube3;

		GameObject* monke;

		Texture2D specularTex;
		Texture2D diffuseTex;
		Texture2D ambientTex;

		MyApplication();
		~MyApplication() override;
		void Update() override;
	};
}

