#include "tetrapc.h"
#include "ComponentManager.h"

using namespace TetraEngine;

std::map<uint, std::vector<std::unique_ptr<Component>>> ComponentManager::components;

void ComponentManager::RemoveComponent(Component* component)
{
	auto loc = &components[component->typeID];
	auto it = std::find_if(loc->begin(), loc->end(), 
		[component](std::unique_ptr<Component>& comp) { return comp->objectID == component->objectID; });
	if (it != loc->end())
	{
		loc->erase(it);
	}
	else
	{
		std::cout << "Component not found\n";
	}
}

Component* ComponentManager::GetComponent(uint typeID, GUID objectID)
{
	auto loc = &components[typeID];
	auto it = std::find_if(loc->begin(), loc->end(), 
		[objectID](std::unique_ptr<Component>& comp) { return comp->objectID == objectID; });
	if (it != loc->end())
	{
		return it->get();
	}
	else
	{
		std::cout << "Component not found\n";
		return nullptr;
	}
}
void TetraEngine::ComponentManager::GetComponents(uint typeID, comp_iter& begin, comp_iter& end)
{
	auto loc = &components[typeID];
	begin = loc->begin();
	end = loc->end();
}

