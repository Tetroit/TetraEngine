//
// Created by pchyo on 04/11/2025.
//

#include "PlaneCollider.h"

#include "../Core.h"

namespace TetraEngine {
    physx::PxShape * PlaneCollider::GetShape() {
        return planeShape;
    }

    PlaneCollider * PlaneCollider::CreateDefault() {
        TETRA_USE_MAIN_PHYSICS_INSTANCE
        return new PlaneCollider(physicsInstance->GetPhysics(), *physicsInstance->GetDefaultMaterial());
    }

    PlaneCollider::PlaneCollider(physx::PxPhysics *physics, physx::PxMaterial &mat) {
        planeShape = physics->createShape(physx::PxPlaneGeometry(), mat);
    }

    PlaneCollider::PlaneCollider(physx::PxMaterial &mat) {
        TETRA_USE_MAIN_PHYSICS
        planeShape = physics->createShape(physx::PxPlaneGeometry(), mat);
    }
} // TetraEngine