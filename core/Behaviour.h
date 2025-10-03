#pragma once

#include <functional>
#include <string>

namespace TetraEngine {
	//class GameObject_new;
	class Behaviour
	{
		//friend class GameObject;

	public:

		//GameObject* gameObject;
		bool destroyWithObject = true;
		std::string tag = "script";

		Behaviour(std::string tag);
		virtual void Update();
		virtual void Start();
	};
}

