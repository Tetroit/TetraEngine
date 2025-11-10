#pragma once

#include <PxPhysicsAPI.h>

namespace TetraEngine {
    class Collider {
        bool isTrigger = false;
    public:
        virtual ~Collider() = default;
        virtual physx::PxShape* GetShape() = 0;
        virtual void MakeTrigger(bool state);
        bool IsTrigger() {
            return isTrigger;
        }
    };
} // TetraEngine