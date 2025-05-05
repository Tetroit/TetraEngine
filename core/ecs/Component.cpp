#include <tetrapc.h>
#include "Component.h"
#include "ComponentManager.h"

using namespace TetraEngine;

GUID ComponentBase::GenerateID()
{
	GUID guid;
	CoCreateGuid(&guid);
	return guid;
}
ComponentBase::ComponentBase(ComponentManager* manager, uint typeID, std::string name) : manager(manager), typeID(typeID), id(GenerateID())
{
}
void ComponentBase::AssignID()
{
}

void ComponentBase::Rename(std::string newName)
{
	name = newName;
}
