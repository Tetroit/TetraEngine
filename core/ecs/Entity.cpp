
#include "Entity.h"

namespace TetraEngine {
    namespace ECS {

        bool Entity::isValid() const {
            return ID != INVALID_ENTITY;
        }
        bool Entity::operator==(const Entity &other) const {
            return ID == other.ID && gen == other.gen;
        }
    } // ECS
} // TetraEngine