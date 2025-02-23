#pragma once

#include <memory>
namespace TetraEngine {
	class Cubemap;
	class Texture2D;
	class Shader;
	class VertexData;

	class Skybox
	{
	public:
		enum Mode {
			BOX,
			SPHERE
		};

		static Skybox* current;
		Skybox(Mode mode = SPHERE, const std::string& path = texturePath + "/skybox.jpg");
		~Skybox();
		void Render();
		Cubemap* cubemap;


	private:
		const Mode mode;
		Texture2D* texture;
		Shader* shader;
		std::shared_ptr<VertexData> mesh;
	};
}

