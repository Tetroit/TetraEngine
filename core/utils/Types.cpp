#include "Types.h"
#include "../Core.h"

using namespace TetraEngine;

const TypeMetadata* TypeRegistry::Get(asset_type_id id) {
    return Core::GetTypeRegistry()->Get_(id);
}