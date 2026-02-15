#pragma once

#include <queue>

#include "../ecs/ECS.h"

namespace TetraEngine {
	class GameObject;

	class DestroyManager
	{
	public:
		std::vector<GameObject*> toDelete;
		Action<GameObject*> OnDestroy;

		void Clear();
		void Update();
		void DeleteAllObjects();
		void Push(GameObject* go);
	};

}