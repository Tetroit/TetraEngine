#pragma once
#include "tetrapc.h"

namespace TetraEngine
{
	class ComponentManager;

	template<class T>
	class Component;
	template<class T>
	class ComponentHandle;

	class ComponentBase
	{
		template<class T>
		friend class ComponentHandle;
		friend class ComponentManager;

		void AssignID();
		static GUID GenerateID();
		GUID id;
		uint typeID;
		uint owner;
		ComponentManager* manager;

	protected:
		ComponentBase(ComponentManager* manager, uint typeID, const std::string& name = "New Component");
		void AssignOwner(uint ownerId)
		{
			owner = ownerId;
		}

	public:

		virtual ~ComponentBase() = default;
		std::string name = "New Component";

		void Rename(std::string newName);
		[[nodiscard]] GUID GetGUID() const;

		template<typename T>
		Component<T>& as()
		{
			int inID = typeid(T).hash_code();

			if (inID != typeID)
			{
				throw std::runtime_error("Invalid component cast");
			}

			return static_cast<Component<T>&>(*this);
		}

		template<typename T>
		Component<T>* asptr()
		{
			return static_cast<Component<T>*>(this);
		}

		bool operator==(const ComponentBase& other) const {
			return (id == other.id);
		}

		[[nodiscard]]
		constexpr uint GetType() const
		{
			return typeID;
		}
		[[nodiscard]]
		constexpr uint GetOwner() const
		{
			return owner;
		}
	};

	template <class T>
	class Component : public ComponentBase
	{

	public:
		std::unique_ptr<T> data;
		template <typename... Args>
		explicit Component(ComponentManager* manager, Args&&... args) :
			ComponentBase(manager, typeid(T).hash_code()), data(std::make_unique<T>(std::forward<Args>(args)...)) {};

		T operator->()  {
			return &data;
		}
	};

}
