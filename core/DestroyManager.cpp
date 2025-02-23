#include "tetrapc.h"
#include "DestroyManager.h"
#include "rendering/Scene.h";

using namespace TetraEngine;

DestroyManager* DestroyManager::get()
{
	static DestroyManager instance;
	return &instance;
}

void DestroyManager::clear()
{
	while (toDelete.size() > 0)
		toDelete.pop();
}

void DestroyManager::deleteAll()
{
	while (toDelete.size() > 0)
	{
		Scene::currentScene->RemoveObject(toDelete.front());
		toDelete.pop();
	}
}

void DestroyManager::push(GameObject* go)
{
	toDelete.push(go);
}
