#pragma once

#include <glm/glm.hpp>

#include "../GameObject.h"
#include "LightRenderer.h"

namespace TetraEngine {
	class LightRenderer;

	class Light : public GameObject
	{
	public:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		void Render() override;
		Light(glm::vec3 pos, const std::string name, LightRenderer* meshRenderer = LightRenderer::defaultRenderer);
		static Light* Create(glm::vec3 pos, const std::string name, LightRenderer* meshRenderer = LightRenderer::defaultRenderer);
	};
}
