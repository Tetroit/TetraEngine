#pragma once

#include "Light.h"
#include "LightRenderer.h"

namespace TetraEngine {
	class Scene;

	class PointLight : public Light
	{
	public:

		glm::vec3 attenuation;
		PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation = glm::vec3(1,0.5,0.2));
	};
}

