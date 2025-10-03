#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "LightRenderer.h"
#include "../GameObject.h"

namespace TetraEngine {
	class PointLight;
	class Shader;
	class Transform;
	class GameObject;

	class LightManager
	{
		struct PointLightInfo {

			glm::vec3 position; float _pad1;
			glm::vec3 ambient; float _pad2;
			glm::vec3 diffuse; float _pad3;
			glm::vec3 specular; float _pad4;
			glm::vec3 attenuation; float _pad5;
		};

		std::vector<PointLightInfo> pointLights;
		GLuint SSBO = -1;
		void AddPointLight(PointLight& pl, Transform& transform, GameObject::Info& gameObject);

	public:
		LightManager();
		~LightManager();

		glm::vec3 mainLightDir = glm::vec3(-1, 1, 1);
		glm::vec3 mainLightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::vec3 mainLightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 mainLightSpecular = glm::vec3(0.3f, 0.3f, 0.3f);
		void dispatchPointLights(Shader* shader);
		void CollectLightData();
	};
}

