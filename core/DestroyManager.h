#pragma once

#include <queue>
#include "../ecs/ECS.h"

namespace TetraEngine {
	class GameObject;

	class DestroyManager
	{
	public:
		std::vector<GameObject*> toDelete;
		void clear();
		void deleteAll();
		void Push(GameObject* go);
	};

}