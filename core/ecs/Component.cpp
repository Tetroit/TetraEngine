#include <tetrapc.h>

#include <utility>
#include "Component.h"
#include "ComponentManager.h"

using namespace TetraEngine;

UUID ComponentBase::GenerateID()
{
	return UUID::Next();
}
ComponentBase::ComponentBase(ComponentManager* manager, uint typeID, const std::string& name) : manager(manager),
	typeID(typeID), owner(0), id(GenerateID()) {
}

void ComponentBase::Rename(std::string newName)
{
	name = std::move(newName);
}

UUID ComponentBase::GetGUID() const {
	return id;
}
