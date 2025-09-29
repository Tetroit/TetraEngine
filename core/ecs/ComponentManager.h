#pragma once

#include "tetrapc.h"
#include "Component.h"
#include "ComponentHandle.h"


namespace TetraEngine
{
	using comp_iter = std::vector<std::unique_ptr<ComponentBase>>::iterator;
	using entity_iter = std::vector<ComponentBase*>::iterator;

	class ComponentManager
	{
		template<class T>
		friend class ComponentHandle;
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
		uint CreateEntity();
		bool RemoveComponent(ComponentBase* component);
		void RemoveEntity(uint entity);

		[[nodiscard]]
		int GetEntityCount() const;
		[[nodiscard]]
		int GetComponentCount() const;

		template<class T>
		[[nodiscard]]
		static uint GetTypeId() {
			return typeid(T).hash_code();
		}

		template<class T>
		[[nodiscard]]
		int GetComponentCountOfType() const{

			const uint typeID = GetTypeId<T>();
			if (!components.contains(typeID)) return 0;
			return components.at(typeID).size();
		}

		template <typename T, typename... Args>
		ComponentHandle<T> CreateComponent(uint entity, Args&&... args)
		{
			const uint typeID = GetTypeId<T>();
			auto& loc = components[typeID];
			auto component = std::make_unique<Component<T>>(this, std::forward<Args>(args)...);
			loc.push_back(std::move(component));
			auto it = std::prev(loc.end());

			entities[entity].push_back(it->get());
			(*it)->AssignOwner(entity);
			return ComponentHandle<T>(component.get());
		}

		template <class T>
		ComponentHandle<T> GetComponent(uint objectID)
		{
			auto raw = GetComponent(GetTypeId<T>(), objectID);
			if (raw == nullptr) return ComponentHandle<T>(nullptr);
			return ComponentHandle<T>(&raw->template as<T>());
		}

		template <class T>
		void GetComponents(comp_iter& begin, comp_iter& end)
		{
			return GetComponents(GetTypeId<T>(), begin, end);
		}

		template <class T>
		bool RemoveComponent(const uint entity) {
			auto comp =  GetComponent(GetTypeId<T>(), entity);
			auto[ptrE, resE] = EraseFromEntities(comp);
			auto[ptrC, resC] = EraseFromComponents(comp);
			return resE && resC;
		}
	};
}
