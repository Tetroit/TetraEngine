#include "tetrapc.h"
#include "Scene.h"

#include "Camera.h"
#include "../Core.h"
#include "../GameObject_old.h"
#include "../GameObject.h"
#include "Skybox.h"
#include "Shader.h"
#include "../Transform.h"
#include "MeshRenderer.h"
#include "../physics/RigidBody.h"
#include "../physics/PhysicsScene.h"
#include "../utils/Time.h"
#include "../DestroyManager.h"

using namespace TetraEngine;

Scene* Scene::currentScene = nullptr;

Scene::Scene() {

	utilizedShaders.clear();
	RegisterShader(Shader::textShader);
	if (currentScene == nullptr)
		currentScene = this;
    physicsScene = std::make_unique<PhysicsScene>();
    lightManager = std::make_unique<LightManager>();
}
Scene::~Scene() {
    Clear();
	delete cameraContext;
}

void Scene::SetActiveScene(Scene *scene) {
    if (currentScene == scene) {
        return;
    }
    if (currentScene == nullptr) {
        TETRA_USE_MAIN_PHYSICS_INSTANCE
        currentScene = scene;
        physicsInstance->SetActiveScene(scene->physicsScene.get());
    }
}

PhysicsScene * Scene::GetPhysicsScene() {
    return physicsScene.get();
}

void Scene::Clear() {
    TETRA_USE_MAIN_ECS
    for (ECS::Entity& e : gameObjects) {
        RemoveObject(e);
        ecs.RemoveEntity(e);
    }
    gameObjects.clear();
}

void Scene::Render() {

	if (cameraContext == nullptr)
		cameraContext = Camera::main;

	lightManager->CollectLightData();
	SetGlobalShaderData();

	if (skybox != nullptr)
		skybox->Render();

	RenderItems();
}

void Scene::RenderItems() {
	Core::GetMainECS().Foreach<MeshRenderer, Transform, GameObjectInfo>(
	    [&](MeshRenderer& mr, Transform& tr, GameObjectInfo& info) {
		RenderItem(info, tr, mr);
	});
}

void Scene::RenderItem(GameObjectInfo& info,  Transform& transform, MeshRenderer& renderer) {
	if (info.isEnabled && info.scene == this) {
		if (transform.IsDirty())
			transform.Recalculate();
		renderer.Render(transform.GetGlobalMatrix());
	}
}

void Scene::Update() {
    physicsScene->Update(Time::deltaTime);
}

void Scene::ParentChangedCallback(ECS::Handle<Transform> &parent, ECS::Handle<Transform> &transform) {

	auto locSubject = std::ranges::find(rootObjects, transform);
	auto locParent = std::ranges::find(rootObjects, parent);

	bool emptyParent = !parent.Valid();
	bool rootSubject = locSubject != rootObjects.end();

	if (!emptyParent && rootSubject) {
		rootObjects.erase(locSubject);
	}
	else if (emptyParent && !rootSubject) {
		rootObjects.push_back(transform);
	}
}

void Scene::AddObject(const GameObject& go) {

	auto info = go.GetInfoHandle();
	auto transform = go.GetTransformHandle();

	AddObject(go.GetEntity(), info, transform);
}

void Scene::RemoveObject(const GameObject& go) {

	auto info = go.GetInfoHandle();
	auto transform = go.GetTransformHandle();

	RemoveObject(go.GetEntity(), info, transform);
}

void Scene::AddObject(const ECS::Entity& go,
                      ECS::Handle<GameObjectInfo> infoH,
                      ECS::Handle<Transform> transformH) {


	auto& ECS = Core::GetMainECS();

	if (!infoH.Valid()) {
		infoH = ECS.GetHandle<GameObjectInfo>(go);
	}
	if (!infoH.Valid()) {
		LOG_ERR("GameObject has no info component");
		return;
	}
	if (!transformH.Valid()) {
		transformH = ECS.GetHandle<Transform>(go);
	}
	if (!transformH.Valid()) {
		LOG_ERR("GameObject has no transform component");
		return;
	}

	auto transform = ECS.GetComponent(transformH);
	auto info = ECS.GetComponent(infoH);

	info->scene = this;

	transform->OnSetParent.AddCallback([this](ECS::Handle<Transform>& parent, ECS::Handle<Transform>& sender) {
			ParentChangedCallback(parent, sender);
		}, "Scene");
	if (transform->GetParent() == nullptr) {
		rootObjects.push_back(transform->GetParentHandle());
	}

    if (ECS.HasStorage<MeshRenderer>()) {
        if (auto comp = ECS.GetComponent<MeshRenderer>(go); comp != nullptr) {
            RegisterShader(comp->shader);
        }
    }
    if (ECS.HasStorage<RigidBody>()) {
        if (auto comp = ECS.GetComponent<RigidBody>(go); comp != nullptr) {
            physicsScene->AddObject(*comp);
        }
    }

	for (auto child: transform->GetChildren()) {
		auto childInfo = ECS.GetRelatedComponent<GameObjectInfo, Transform>(child);
		AddObject(childInfo->entity);
	}
}

void Scene::RemoveObject(const ECS::Entity &go,
	ECS::Handle<GameObjectInfo> infoH,
	ECS::Handle<Transform> transformH) {


	auto& ECS = Core::GetMainECS();

	if (!infoH.Valid()) {
		infoH = ECS.GetHandle<GameObjectInfo>(go);
	}
	if (!infoH.Valid()) {
		LOG_ERR("GameObject has no info component");
		return;
	}
	if (!transformH.Valid()) {
		transformH = ECS.GetHandle<Transform>(go);
	}
	if (!transformH.Valid()) {
		LOG_ERR("GameObject has no transform component");
		return;
	}

	auto transform = ECS.GetComponent(transformH);
	auto info = ECS.GetComponent(infoH);

	info->scene = nullptr;

	transform->OnSetParent.RemoveCallback("Scene");
	auto loc = std::ranges::find(rootObjects, transformH);
	if (loc != rootObjects.end()) {
		rootObjects.erase(loc);
	}

	if (auto comp = ECS.GetComponent<MeshRenderer>(go); comp != nullptr) {
		DeregisterShader(comp->shader);
	}
	if (auto comp = ECS.GetComponent<RigidBody>(go); comp != nullptr) {
		TETRA_USE_MAIN_PHYSICS_INSTANCE
	    physicsScene->RemoveObject(*comp);
	}

	for (auto child: transform->GetChildren()) {
		auto childInfo = ECS.GetRelatedComponent<GameObjectInfo, Transform>(child);
		RemoveObject(childInfo->entity);
	}
}

void Scene::RegisterShader(Shader* shader)
{
	auto iterator = utilizedShaders.find(shader);
	if (iterator != utilizedShaders.end())
	{
		iterator->second++;
		return;
	}

	utilizedShaders.emplace(shader, 1);
}

void Scene::DeregisterShader(Shader* shader)
{
	auto iterator = utilizedShaders.find(shader);
	if (iterator != utilizedShaders.end())
	{
		if (iterator->second == 1)
			utilizedShaders.erase(iterator);
		else
			iterator->second--;
		return;
	}
	throw std::out_of_range("tried to remove shader, but no such shader exists");
}

void Scene::SetGlobalShaderData()
{
	if (utilizedShaders.size() == 0) return;
	for (auto key : utilizedShaders)
	{
		Shader* shader = key.first;
		shader->Use();

		shader->SetVec3("viewPos", cameraContext->Position);
		lightManager->dispatchPointLights(shader);
	}
}

void Scene::Render(Camera* cam)
{
	cameraContext = cam;
	Render();
}
