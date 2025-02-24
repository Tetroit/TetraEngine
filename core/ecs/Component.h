#pragma once
#include <tetrapc.h>

namespace TetraEngine
{
	class Component
	{
		void AssignID();
	public:

		Component(uint typeID, GUID objectID) : typeID(typeID), objectID(objectID) {};
		template<typename T>
		Component(uint objectID) : objectID(objectID), typeID(typeid(T).hash_code()) {};
		template<typename T>
		Component() : objectID(AssignID()), typeID(typeid(T).hash_code()) {};

		uint typeID;
		GUID objectID;
		
		template<typename T>
		T&& as()
		{
			if (typeid(T).hash_code() != typeID)
			{
				throw std::exception("Invalid component cast");
			}
			return static_cast<T&&>(*this);
		}

		bool operator==(const Component& other)
		{
			return objectID == other.objectID;
		}
	};

}

