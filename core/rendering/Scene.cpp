#include "tetrapc.h"
#include "Scene.h"

#include <iostream>

#include "Camera.h"
#include "../Core.h"
#include "../GameObject.h"
#include "Skybox.h"
#include "Shader.h"

using namespace TetraEngine;

Scene* Scene::currentScene = nullptr;

Scene::Scene() {

	objects.clear();
	toDelete.clear();
	utilizedShaders.clear();
	RegisterShader(Shader::textShader);
	if (currentScene == nullptr)
		currentScene = this;
}
Scene::~Scene() {
	for (int i = 0; i < objects.size(); i++)
		objects[i]->scene = nullptr;
	objects.clear();
	toDelete.clear();
	delete cameraContext;
}
void Scene::Render() {

	if (cameraContext == nullptr)
		cameraContext = Camera::main;

	SetGlobalShaderData();
	if (skybox != nullptr)
	{
		skybox->Render();
	}
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->parent == nullptr && objects[i])
			objects[i]->Render();
	}
}
void Scene::Update() {
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->parent == nullptr)
			objects[i]->Update();
	}
	DeleteObjects();
}
void Scene::AddObject(GameObject* go) {

	std::unique_ptr<GameObject> ptr(go);

	go->scene = this;
	go->OnSceneAdded(this);

	RegisterShader(go->renderer->shader);

	objects.push_back(std::move(ptr));

	for (GameObject* child : go->children)
		AddObject(child);
}
int Scene::FindObject(GameObject* go) {
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i].get() == go)
			return i;
	}
	return -1;
}
void Scene::DeleteObjects()
{
	for (GameObject* go : toDelete)
	{
		DeregisterShader(go->renderer->shader);
		go->OnSceneRemoved();
		int pos = FindObject(go);
		if (pos != -1)
			objects.erase(objects.begin() + pos);
		else {
			std::cout << "Scene::RemoveObject:\tobject not found\n";
			return;
		}
	}
	toDelete.clear();
}
void Scene::RemoveObject(GameObject* go) {

	for (int i = 0; i < toDelete.size(); i++)
	{
		if (toDelete[i] == go)
			return;
	}
	toDelete.push_back(go);
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
		lightManager.fetchPointLights(shader);
	}
}

void Scene::Render(Camera* cam)
{
	cameraContext = cam;
	Render();
}
