#pragma once

#include <PxPhysicsAPI.h>
#include "Collider.h"

using namespace physx;
namespace TetraEngine {


    class BoxCollider : public Collider {
        PxShape* boxShape = nullptr;
    public:

        static BoxCollider* CreateDefault();

        BoxCollider(PxReal a, PxReal b, PxReal c, const PxMaterial& mat);
        BoxCollider(PxPhysics* physics, PxReal a, PxReal b, PxReal c, const PxMaterial& mat);
        PxShape* GetShape() override;
    };
} // TetraEngine
