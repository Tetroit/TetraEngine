// #pragma once
//
// #include "../rendering/MeshRenderer.h"
//
// namespace TetraEngine {
//
// 	class VertexData;
// 	class Shader;
//
// 	class LightRenderer : public MeshRenderer
// 	{
// 		friend class Light;
// 	protected:
// 		LightRenderer(std::shared_ptr<VertexData> vd, Shader* sh);
//
// 	public:
//
// 		static void InitialiseRenderer();
// 		static LightRenderer* defaultRenderer;
// 		LightRenderer();
// 		void Render(glm::mat4 transform = glm::mat4(1)) override;
// 	};
// }
