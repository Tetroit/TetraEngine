//
// Created by pchyo on 27/10/2025.
//

#include "Collider.h"

namespace TetraEngine {
    void Collider::MakeTrigger(bool state) {
        auto* sh = GetShape();
        sh->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, state);
    }
} // TetraEngine