#include "tetrapc.h"
#include "DestroyManager.h"
#include "rendering/Scene.h"

using namespace TetraEngine;

void DestroyManager::Clear()
{
	toDelete.clear();
}

void DestroyManager::Update() {
	DeleteAllObjects();
}

void DestroyManager::DeleteAllObjects()
{
	TETRA_USE_MAIN_ECS
	while (!toDelete.empty())
	{
		auto obj = toDelete.back();
		OnDestroy.Call(obj);
		Scene::currentScene->RemoveObject(*toDelete.back());
		auto tr = obj->GetTransform();
		if (tr->GetParent() != nullptr) {
			tr->ClearParent();
		}
		delete obj;
	    toDelete.pop_back();
	}
}

void DestroyManager::Push(GameObject* go)
{
    if (go != nullptr)
	toDelete.push_back(go);
}