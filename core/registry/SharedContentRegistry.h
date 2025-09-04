#pragma once
#include <tetrapc.h>

using namespace TetraEngine;

namespace TetraEngine {
	class SharedContentRegistry
	{
		std::map<type_id, std::vector<std::shared_ptr<void>>> registry;
	public:

		SharedContentRegistry() {};
		template <typename T>
		void AddItem(T* item)
		{
			registry[TypeInfo<T>::id].push_back(std::make_shared<T>(std::move(*item)));
		};
		/*template <typename T, typename... Args>
		std::weak_ptr<T> CreateItem(Args&&... args)
		{
			std::shared_ptr<T>& ptr = registry[TypeInfo<T>::id].emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
			return ptr;
		};*/
	};
}

