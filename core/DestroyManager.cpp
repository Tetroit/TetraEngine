#include "tetrapc.h"
#include "DestroyManager.h"
#include "rendering/Scene.h"

using namespace TetraEngine;

void DestroyManager::clear()
{
	toDelete.clear();
}

void DestroyManager::deleteAll()
{
	while (!toDelete.empty())
	{
		Scene::currentScene->RemoveObject(*toDelete.back());
	    toDelete.pop_back();
	}
}

void DestroyManager::Push(GameObject* go)
{
    if (go != nullptr)
	toDelete.push_back(go);
}
