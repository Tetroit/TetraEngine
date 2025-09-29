#include "tetrapc.h"
#include "ComponentManager.h"

using namespace TetraEngine;

ComponentManager* ComponentManager::current = nullptr;

ComponentManager::ComponentManager() {
	if (current == nullptr)
	{
		current = this;
	}
	entities.clear();
}

uint ComponentManager::CreateEntity()
{
	auto [it, generated] = entities.emplace(latestEntity++, std::vector<ComponentBase*>());
	return it->first;
}

bool ComponentManager::RemoveComponent(ComponentBase* component)
{
	auto [itE, successE] = EraseFromEntities(component);
	auto [itC, successC] = EraseFromComponents(component);
	return successC && successE;
}

void ComponentManager::RemoveEntity(uint entity)
{
	auto it = entities.find(entity);
	if (it == entities.end())
	{
		LOG_ERR("Entity not found");
		return;
	}
	auto compArray = &it->second;
	if (compArray->empty())
	{
		LOG_FROM("ECS", "This entity has no components");
		entities.erase(it);
		return;
	}
	int comps = 0;
	for (auto entityIt = compArray->begin(); entityIt != compArray->end(); entityIt++)
	{
		auto [compIt, compSuccess] = EraseFromComponents(*entityIt);
		if (compSuccess)
			comps++;
	}
	entities.erase(it);
	LOG_FROM("ECS", comps << " elements were removed");
}

int ComponentManager::GetEntityCount() const {
	return entities.size();
}

int ComponentManager::GetComponentCount() const {
	int total = 0;
	for (auto loc = entities.begin(); loc != entities.end(); loc++)
		total += loc->second.size();
	return total;
}

ComponentBase* ComponentManager::GetComponent(uint typeID, uint entity)
{
	auto loc = &entities[entity];
	auto it = std::ranges::find_if(*loc, [typeID](const ComponentBase* item) { return item->typeID == typeID; });
	if (it != loc->end())
	{
		return *it;
	}

	LOG_ERR("Component not found while accessing");
	return nullptr;
}

// ComponentBase* ComponentManager::GetComponent(uint typeID, uint entity)
// {
// 	auto loc = &components[typeID];
// 	auto it = std::ranges::find_if(*loc, [entity](const std::unique_ptr<ComponentBase>& item) { return item->owner == entity; });
// 	if (it != loc->end())
// 	{
// 		return it->get();
// 	}
//
// 	LOG_ERR("Component not found while accessing");
// 	return nullptr;
// }

void ComponentManager::GetComponents(uint typeID, comp_iter& begin, comp_iter& end)
{
	auto loc = &components[typeID];
	begin = loc->begin();
	end = loc->end();
}

std::pair<comp_iter,bool> ComponentManager::EraseFromComponents(ComponentBase* component)
{
	auto& loc = components[component->GetType()];

	auto it = std::find_if(loc.begin(), loc.end(),
		[component](const std::unique_ptr<ComponentBase>& comp)
		{return (*comp) == *component;});

	if (it == loc.end())
	{
		LOG_ERR("ECS::Component not found in component array while removing");
		return std::make_pair(it,false);
	}
	return std::make_pair(loc.erase(it), true);
}
std::pair<entity_iter, bool> ComponentManager::EraseFromEntities(ComponentBase* component)
{
	auto& loc = entities[component->GetOwner()];
	auto it = std::ranges::find(loc, component);
	if (it == loc.end())
	{
		LOG_ERR("ECS::Component not found in entity array while removing");
		return std::make_pair(it, false);
	}
	return std::make_pair(loc.erase(it), true);
}

