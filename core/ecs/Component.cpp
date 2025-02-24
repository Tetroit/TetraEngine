#include <tetrapc.h>
#include "Component.h"

using namespace TetraEngine;

void Component::AssignID()
{
	CoCreateGuid(&objectID);
}
