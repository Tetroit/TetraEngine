#pragma once

#include <memory>
#include <glm/glm.hpp>

#define DIFFUSE_TEXTURE_BIT 0x1
#define SPECULAR_TEXTURE_BIT 0x2
#define EMISSION_TEXTURE_BIT 0x4

namespace TetraEngine
{
	class Camera;
	class Texture2D;
	class Material;
	class Shader;
	class VertexData;

	class MeshRenderer
	{

	public:
		static MeshRenderer* defaultRenderer;
		static MeshRenderer* skyboxRenderer;

		std::shared_ptr<VertexData> mesh;
		Texture2D* diffuseTexture;
		Texture2D* specularTexture;
		Texture2D* emissionTexture;
		int textureFlags;
		Shader* shader;
		TetraEngine::Camera* camera;
		Material* material;
		virtual void Render(glm::mat4 transform = glm::mat4(1));
		MeshRenderer(std::shared_ptr<VertexData> vd, Shader* sh);
		void setTexture(Texture2D* texture, int texBit = 1);
		void setTexture(const std::string& name);
		static void InitialiseRenderer();
	};

}
