#pragma once

#include <memory>
#include <glm/glm.hpp>

#define DIFFUSE_TEXTURE_BIT 0x1
#define SPECULAR_TEXTURE_BIT 0x2
#define EMISSION_TEXTURE_BIT 0x4

namespace TetraEngine {
	class ViewProvider;
}

namespace TetraEngine
{
	class ViewportCamera;
	class Texture2D;
	class Material;
	class Shader;
	class VertexData;

	class MeshRenderer
	{
		int textureFlags;

	public:
		static MeshRenderer* defaultRenderer;
		static MeshRenderer* skyboxRenderer;

		std::shared_ptr<VertexData> mesh;
		Texture2D* textureDiffuse;
		Texture2D* textureSpecular;
		Texture2D* textureEmission;

		glm::vec4 scaleOffsetDiffuse = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
		glm::vec4 scaleOffsetSpecular = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
		glm::vec4 scaleOffsetEmission = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);

		Shader* shader;
		ViewProvider* camera;
		Material* material;

		bool isRegistered = false;

		void Render(glm::mat4 transform = glm::mat4(1));
		MeshRenderer(std::shared_ptr<VertexData> vd, Shader* sh);
		void setTexture(Texture2D* texture, int texBit = 1);
		void setTexture(const std::string& name);
		static void InitialiseRenderer();
	};

}
