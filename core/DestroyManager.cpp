#include "tetrapc.h"
#include "DestroyManager.h"
#include "rendering/Scene.h"

using namespace TetraEngine;

DestroyManager* DestroyManager::get()
{
	static DestroyManager instance;
	return &instance;
}

void DestroyManager::clear()
{
	while (!toDelete.empty())
		toDelete.pop();
}

void DestroyManager::deleteAll()
{
	while (!toDelete.empty())
	{
		//Scene::currentScene->RemoveObject(toDelete.front());
		toDelete.pop();
	}
}

void DestroyManager::push(ECS::Handle<GameObject> go)
{
	toDelete.push(go);
}
