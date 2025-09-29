#include <tetrapc.h>

#include <utility>
#include "Component.h"
#include "ComponentManager.h"

using namespace TetraEngine;

GUID ComponentBase::GenerateID()
{
	GUID guid;
	CoCreateGuid(&guid);
	return guid;
}
ComponentBase::ComponentBase(ComponentManager* manager, uint typeID, const std::string& name) : manager(manager),
	typeID(typeID), owner(0), id(GenerateID()) {
}

void ComponentBase::Rename(std::string newName)
{
	name = std::move(newName);
}

GUID ComponentBase::GetGUID() const {
	return id;
}
