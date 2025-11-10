#pragma once
#include "Collider.h"

namespace TetraEngine {
    class PlaneCollider : public Collider {
        physx::PxShape* planeShape;
    public:
        physx::PxShape *GetShape() override;
        [[nodiscard]] static PlaneCollider* CreateDefault();
        PlaneCollider(physx::PxPhysics *physics, physx::PxMaterial& mat);
        PlaneCollider(physx::PxMaterial& mat);
    };
} // TetraEngine