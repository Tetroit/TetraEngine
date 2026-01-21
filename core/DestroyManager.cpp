#include "tetrapc.h"
#include "DestroyManager.h"
#include "rendering/Scene.h"

using namespace TetraEngine;

void DestroyManager::Clear()
{
	toDelete.clear();
	actorsToRemove.clear();
}

void DestroyManager::Update() {
	RemoveActors();
	DeleteAllObjects();
}

void DestroyManager::RemoveActors() {
	int cnt = 0;
	for (auto [scene, actor] : actorsToRemove) {
		scene->removeActor(*actor);
		cnt++;
	}
	actorsToRemove.clear();
	if (cnt > 0) {
		LOG_FROM("DESTROY_MANAGER", "Removed " << cnt << " actors");
	}
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
		auto rb = obj->GetComponent<RigidBody>();
		if (rb != nullptr) {
			auto it = std::find_if(actorsToRemove.begin(), actorsToRemove.end(),
			[rb](const std::pair<PxScene*, PxActor*>& p)
			{
				return p.second == rb->GetActor();
			});
			if (it!=actorsToRemove.end()) {
				actorsToRemove.erase(it);
			}
			rb->DestroyImmediate();
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
void DestroyManager::Push(PxScene* scene, PxActor* actor)
{
	if (scene != nullptr && actor != nullptr && actor->getScene() == scene)
		actorsToRemove.emplace_back(scene, actor);
}
