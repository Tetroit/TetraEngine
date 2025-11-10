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
    class PhysicsScene;
    class DestroyManager;

	class Scene
	{
		std::unique_ptr<LightManager> lightManager;
	    std::unique_ptr<PhysicsScene> physicsScene;

	public:
		static Scene* currentScene;

		std::string name;
		std::map<Shader*, int> utilizedShaders;
		Skybox* skybox = nullptr;
		Camera* cameraContext = nullptr;
		std::vector<ECS::Handle<Transform>> rootObjects;
	    std::vector<ECS::Entity> gameObjects;

		Scene();
		~Scene();

	    static void SetActiveScene(Scene* scene);
        PhysicsScene* GetPhysicsScene();

	    void Clear();
		void AddObject(const GameObject& go);
		void RemoveObject(const GameObject& go);
		void AddObject(const ECS::Entity& go,
			ECS::Handle<GameObjectInfo> infoH = ECS::Handle<GameObjectInfo>::CreateInvalid(),
			ECS::Handle<Transform> transformH = ECS::Handle<Transform>::CreateInvalid());
		void RemoveObject(const ECS::Entity& go,
			ECS::Handle<GameObjectInfo> infoH = ECS::Handle<GameObjectInfo>::CreateInvalid(),
			ECS::Handle<Transform> transformH = ECS::Handle<Transform>::CreateInvalid());

		void RegisterShader(Shader* shader);
		void DeregisterShader(Shader* shader);
		void SetGlobalShaderData();
		void Render(Camera* cam);
		void Render();
		void RenderItems();
		void RenderItem(GameObjectInfo& info, Transform& transform, MeshRenderer& renderer);

		void Update();

		void ParentChangedCallback(ECS::Handle<Transform>& transform, ECS::Handle<Transform>& parent);

	private:

	};
}

