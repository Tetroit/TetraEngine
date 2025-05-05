#pragma once
#include <tetrapc.h>

namespace TetraEngine
{
	class ComponentManager;

	template<typename T>
	class Component;

	class ComponentBase
	{
		friend class ComponentManager;

		void AssignID();
		GUID GenerateID();
		GUID id;
		uint typeID;
		uint owner;
		ComponentManager* manager;

	protected:
		ComponentBase(ComponentManager* manager, uint typeID, std::string name = "New Component");
		void AssignOwner(uint id)
		{
			owner = id;
		}

	public:

		virtual ~ComponentBase() = default;
		std::string name = "New Component";

		void Rename(std::string newName);

		template<typename T>
		Component<T>&& as()
		{
			int inID = typeid(T).hash_code();

			if (inID != typeID)
			{
				throw std::runtime_error("Invalid component cast");
			}

			return static_cast<Component<T>&&>(*this);
		}

		template<typename T>
		Component<T>* asptr()
		{
			return static_cast<Component<T>*>(this);
		}

		bool operator==(const ComponentBase& other)
		{
			return (id == other.id);
		}

		constexpr uint GetType() const
		{
			return typeID;
		}
		constexpr uint GetOwner() const
		{
			return owner;
		}
	};

	template <typename T>
	class Component : public ComponentBase
	{
	public:
		std::unique_ptr<T> data;
		template <typename... Args>
		Component(ComponentManager* manager, Args&&... args) : 
			ComponentBase(manager, typeid(T).hash_code()), data(std::make_unique<T>(std::forward<Args>(args)...)) {};
	};
}