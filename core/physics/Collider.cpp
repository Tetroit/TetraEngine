//
// Created by pchyo on 27/10/2025.
//

#include "Collider.h"

#include "../Core.h"
#include "../rendering/VertexData.h"

namespace TetraEngine {
    void Collider::ResolveProperties(PxPhysics *&physics, PxMaterial *&mat) {
        if (physics == nullptr) {
            physics = Core::GetPhysics();
        }
        if (mat == nullptr) {
            mat = Core::GetPhysicsInstance()->GetDefaultMaterial();
        }
    }

    Collider::Collider(bool isTrigger) : isTrigger(isTrigger) {
    }

    PxShape * Collider::GetShape() {
        return shapes[0];
    }

    void Collider::MakeTrigger(bool state) {
        auto* sh = GetShape();
        sh->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !state);
        sh->setFlag(PxShapeFlag::eTRIGGER_SHAPE, state);
    }

    bool Collider::IsTrigger() const {
        return isTrigger;
    }

    uint Collider::AddBox(PxReal a, PxReal b, PxReal c, PxPhysics* physics, PxMaterial* mat) {
        ResolveProperties(physics, mat);
        shapes.push_back(physics->createShape(PxBoxGeometry(a,b,c), *mat));
        shapeTypes.push_back(BOX);
        uint id = static_cast<uint>(shapes.size()) - 1;
        if (id == 0)
            MakeTrigger(isTrigger);
        return id;
    }

    uint Collider::AddBox(PxReal a, PxPhysics *physics, PxMaterial *mat) {
        return AddBox(a,a,a, physics, mat);
    }

    uint Collider::AddSphere(PxReal r, PxPhysics *physics, PxMaterial *mat) {
        ResolveProperties(physics, mat);
        shapes.push_back(physics->createShape(PxSphereGeometry(r), *mat));
        shapeTypes.push_back(SPHERE);
        uint id = static_cast<uint>(shapes.size()) - 1;
        if (id == 0)
            MakeTrigger(isTrigger);
        return id;
    }

    // uint Collider::AddConvexMesh(const VertexData &data) {
    //
    // }
} // TetraEngine