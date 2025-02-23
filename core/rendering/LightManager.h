#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace TetraEngine {
	class PointLight;
	class Shader;

	class LightManager
	{
	public:

		std::vector<PointLight*> pointLights;
		glm::vec3 mainLightDir = glm::vec3(-1, 1, 1);
		glm::vec3 mainLightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::vec3 mainLightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 mainLightSpecular = glm::vec3(0.3f, 0.3f, 0.3f);
		void fetchPointLights(Shader* shader);
		int AddPointLight(PointLight* pl);
		void RemovePointLight(int id);
	};
}

