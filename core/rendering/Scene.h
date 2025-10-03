#pragma once

#include "LightManager.h"
#include <string>
#include <memory>
#include <map>

#include "../GameObject.h"
#include "../ecs/ECS.h"

namespace TetraEngine
{
	class Camera;
	class Skybox;
	class GameObject;
	class Shader;
	class Transform;
	class MeshRenderer;

	class Scene
	{
	public:
		static Scene* currentScene;

		std::string name;
		// std::vector<std::unique_ptr<GameObject>> objects;
		std::vector<GameObject> objects_new;
		std::vector<ECS::Entity> entities;
		// std::vector<GameObject*> toDelete;
		std::map<Shader*, int> utilizedShaders;
		LightManager lightManager;
		Skybox* skybox = nullptr;
		Camera* cameraContext = nullptr;

		Scene();
		~Scene();
		void AddObject(GameObject* go);
		void RegisterShader(Shader* shader);
		void DeregisterShader(Shader* shader);
		void SetGlobalShaderData();
		void Render(Camera* cam);
		void Render();
		void Render_new();
		void RenderItem_new(GameObject::Info& info, Transform& transform, MeshRenderer& renderer);

		void Update();

	private:

	};
}

