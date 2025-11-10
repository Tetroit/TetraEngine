//
// Created by pchyo on 29/10/2025.
//

#include <PxPhysicsAPI.h>
#include "BoxCollider.h"
#include "../Core.h"

namespace TetraEngine {
    BoxCollider* BoxCollider::CreateDefault() {
        TETRA_USE_MAIN_PHYSICS_INSTANCE
        return new BoxCollider(0.5f, 0.5f, 0.5f, *physicsInstance->GetDefaultMaterial());
    }
    BoxCollider::BoxCollider(PxReal a, PxReal b, PxReal c, const PxMaterial &mat) : BoxCollider(Core::GetPhysics(), a, b, c, mat) {}
    BoxCollider::BoxCollider(PxPhysics *physics, PxReal a, PxReal b, PxReal c, const PxMaterial &mat) {
        boxShape = physics->createShape(PxBoxGeometry(a,b,c), mat);
    }

    PxShape * BoxCollider::GetShape() {
        return boxShape;
    }
} // TetraEngine