#pragma once
#include <tetrapc.h>
#include "Component.h"
#include <combaseapi.h>

namespace TetraEngine
{
	using comp_iter = std::vector<std::unique_ptr<Component>>::iterator;

	static class ComponentManager
	{
		static std::map<uint, std::vector<std::unique_ptr<Component>>> components;


		static Component* GetComponent(uint typeID, GUID objectID);
		static void GetComponents(uint typeID, comp_iter& begin, comp_iter& end);

	public:

		template <typename T>
		static void AddComponent(Component* component) {
			components[component->typeID].push_back(std::make_unique<T>(std::move(component-> template as<T>())));
		}

		static void RemoveComponent(Component* component);

		template <typename T>
		inline static Component* GetComponent(GUID objectID)
		{
			return GetComponent(typeid(T).hash_code(), objectID);
		}

		template <typename T>
		inline static std::vector<Component*> GetComponents() 
		{
			return GetComponents(typeid(T).hash_code());
		}
		/// <summary>
		/// WARNING: THIS WILL REMOVE ALL COMPONENTS FROM THE COMPONENT MANAGER
		/// not recommended for use
		/// </summary>
		static void ClearAllComponents();
		static void ClearComponents(uint typeID);
	};
}

