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

using namespace TetraEngine;

Scene* Scene::currentScene = nullptr;

Scene::Scene() {

	utilizedShaders.clear();
	RegisterShader(Shader::textShader);
	if (currentScene == nullptr)
		currentScene = this;
}
Scene::~Scene() {
	delete cameraContext;
}
void Scene::Render() {

	if (cameraContext == nullptr)
		cameraContext = Camera::main;

	lightManager.CollectLightData();
	SetGlobalShaderData();

	if (skybox != nullptr)
		skybox->Render();

	Render_new();
}

void Scene::Render_new() {
	Core::GetMainECS().Foreach<MeshRenderer, Transform, GameObject::Info>([&](MeshRenderer& mr, Transform& tr, GameObject::Info& info) {
		RenderItem_new(info, tr, mr);
	});
}

void Scene::RenderItem_new(GameObject::Info& info,  Transform& transform, MeshRenderer& renderer) {
	if (info.isEnabled && info.scene == this) {
		if (transform.IsDirty())
			transform.Recalculate();
		renderer.Render(transform.GetGlobalMatrix());
	}
}

void Scene::Update() {

	//DeleteObjects();
}

void Scene::AddObject(GameObject *go) {

	go->SetScene(this);
	if (auto comp = go->GetComponent<MeshRenderer>(); comp != nullptr) {
		RegisterShader(comp->shader);
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
		lightManager.dispatchPointLights(shader);
	}
}

void Scene::Render(Camera* cam)
{
	cameraContext = cam;
	Render();
}
