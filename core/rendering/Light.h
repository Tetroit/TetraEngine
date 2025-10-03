#pragma once

#include <glm/glm.hpp>

#include "../GameObject_old.h"
#include "LightRenderer.h"

namespace TetraEngine {
	class LightRenderer;
	class LightManager;

	class Light
	{

	public:

		enum LightType {
			INVALID = 0,
			DIRECTIONAL = 1,
			POINT = 2,
		};

	protected:
		LightType lightType;

	public:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		Light(LightType type, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
		explicit Light(LightType type) : Light(type, glm::vec3(0.1f), glm::vec3(0.3f), glm::vec3(0.9f)) {}
	};
}
