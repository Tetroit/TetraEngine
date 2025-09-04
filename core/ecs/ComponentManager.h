#pragma once
#include <tetrapc.h>
#include <combaseapi.h>
#include "Component.h"


namespace TetraEngine
{
	using comp_iter = std::vector<std::unique_ptr<ComponentBase>>::iterator;
	using entity_iter = std::vector<ComponentBase*>::iterator;

	class ComponentManager
	{
	private:

		std::map<uint, std::vector<std::unique_ptr<ComponentBase>>> components;
		std::unordered_map<uint, std::vector<ComponentBase*>> entities;

		ComponentBase* GetComponent(uint typeID, uint objectID);
		void GetComponents(uint typeID, comp_iter& begin, comp_iter& end);
		std::pair<entity_iter,bool> EraseFromEntities(ComponentBase* component);
		std::pair<comp_iter,bool> EraseFromComponents(ComponentBase* component);
		uint latestEntity = 0;
	public:

		static ComponentManager* current;
		ComponentManager();

		template <typename T, typename... Args>
		Component<T>* CreateComponent(uint entity, Args&&... args)
		{
			uint typeID = typeid(T).hash_code();
			auto& loc = components[typeID];
			std::unique_ptr<Component<T>> component = std::make_unique<Component<T>>(this, std::forward<Args>(args)...);
			loc.push_back(std::move(component));
			auto it = std::prev(loc.end());

			entities[entity].push_back(it->get());
			it->get()->AssignOwner(entity);
			return it->get()->template asptr<T>();
		}
		uint CreateEntity();

		bool RemoveComponent(ComponentBase* component);
		void RemoveEntity(uint entity);

		template <typename T>
		inline ComponentBase* GetComponent(uint objectID)
		{
			return GetComponent(typeid(T).hash_code(), objectID);
		}

		template <typename T>
		inline void GetComponents(comp_iter& begin, comp_iter& end)
		{
			return GetComponents(typeid(T).hash_code(), begin, end);
		}

		// template <typename T>
		// inline std::vector<ComponentBase*> GetComponents()
		// {
		// 	return GetComponents(typeid(T).hash_code());
		// }
	};
}

