#pragma once

#include "LightManager.h"
#include <string>
#include <memory>
#include <map>

#include "../GameObject.h"
#include "../ecs/ECS.h"

namespace TetraEngine
{
	class ViewportCamera;
	class Skybox;
	class GameObject;
	class Shader;
	class Transform;
	class MeshRenderer;
    class PhysicsScene;
	class DestroyManager;
	class Camera;
	class ViewProvider;

	class Scene
	{
		std::unique_ptr<LightManager> lightManager;
	    std::unique_ptr<PhysicsScene> physicsScene;
		std::unique_ptr<ViewportCamera> viewportCamera;
		ViewProvider* cameraContext = nullptr;

	public:
		static Scene* currentScene;

		std::string name;
		std::map<Shader*, int> utilizedShaders;
		Skybox* skybox = nullptr;
		Camera* gameCamera = nullptr;
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
		void Render(ViewProvider* cam);
		void Render();
		void RenderItems();
		void RenderItem(GameObjectInfo& info, Transform& transform, MeshRenderer& renderer);

		void Update();

		void ParentChangedCallback(ECS::Handle<Transform>& transform, ECS::Handle<Transform>& parent);

		void SwitchToGameView();
		void SwitchToEditorView();

	private:

	};
}

