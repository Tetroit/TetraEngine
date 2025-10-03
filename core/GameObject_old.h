// #pragma once
//
// #include <vector>
// #include <string>
//
// #include "rendering/MeshRenderer.h"
//
// namespace TetraEngine {
// 	class Scene;
// 	class Behaviour;
//
// 	class GameObject
// 	{
// 	private:
// 		static glm::mat4 currentTransform;
// 	public:
// 		std::string name;
// 		glm::mat4 transform;
// 		glm::mat4 localToGlobal;
// 		Scene* scene;
// 		GameObject* parent;
// 		MeshRenderer* renderer;
// 		std::vector<GameObject*> children;
// 		std::vector<Behaviour*> scripts;
//
// 		GameObject(glm::vec3 pos, const std::string name = "object", MeshRenderer* meshRenderer = MeshRenderer::defaultRenderer);
// 		~GameObject();
//
// 		static GameObject* Create(glm::vec3 pos, const std::string name = "object", MeshRenderer* meshRenderer = MeshRenderer::defaultRenderer);
// 		void Delete();
// 		void Rename(const std::string);
// 		void UpdateMatrix();
// 		void AddChild(GameObject* child);
// 		void AddBehaviour(Behaviour* script);
// 		virtual void OnSceneAdded(Scene* scene);
// 		virtual void OnSceneRemoved();
// 		virtual void Render();
// 		void Update();
//
// 		void LocalTranslate(glm::vec3 pos);
// 		void LocalRotate(glm::quat rot);
// 		void LocalScale(glm::vec3 pos);
//
// 		void GlobalTranslate(glm::vec3 pos);
// 		void GlobalRotate(glm::quat rot);
// 		void GlobalScale(glm::vec3 pos);
//
// 		void SetPosition(glm::vec3 pos);
// 		void SetRotation(glm::quat rot);
// 		void SetScale(glm::vec3 scale);
//
// 		glm::vec3 GetPos();
// 		glm::quat GetRotation();
// 		glm::vec3 GetScale();
//
// 		glm::vec3 GetGlobalPos();
// 		glm::quat GetGlobalRotation();
// 		glm::vec3 GetGlobalScale();
//
// 		glm::vec3 TransformPoint(glm::vec3 point);
// 		glm::vec3 TransformDirection(glm::vec3 point);
//
// 		bool HasScript(std::string tag);
// 	};
// }
