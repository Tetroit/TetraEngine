#pragma once

#include <queue>
#include "../ecs/ECS.h"

namespace TetraEngine {
	class GameObject;

	class DestroyManager
	{
	public:
		static DestroyManager* get();
		std::queue<ECS::Handle<GameObject>> toDelete;
		void clear();
		void deleteAll();
		void push(ECS::Handle<GameObject> go);

	};

}