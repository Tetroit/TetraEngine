#pragma once

#include <queue>

#include "PxActor.h"
#include "../ecs/ECS.h"

namespace TetraEngine {
	class GameObject;

	class DestroyManager
	{
		std::vector<std::pair<physx::PxScene*, physx::PxActor*>> actorsToRemove;
	public:
		std::vector<GameObject*> toDelete;
		Action<GameObject*> OnDestroy;

		void Clear();
		void Update();
		void RemoveActors();
		void DeleteAllObjects();
		void Push(GameObject* go);
		void Push(physx::PxScene* scene, physx::PxActor* go);
	};

}