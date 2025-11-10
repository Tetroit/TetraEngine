//
// Created by pchyo on 07/11/2025.
//

#include "PhysicsCallback.h"
#include "../ecs/ECS.h"
#include "RigidBody.h"
#include "../Core.h"

namespace TetraEngine {
    void PhysicsCallback::onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) {
        TETRA_USE_MAIN_ECS
        auto* comp1 = reinterpret_cast<ECS::Handle<RigidBody>*>(pairHeader.actors[0]->userData);
        auto* rb1 = ecs.GetComponent(*comp1);
        auto* comp2 = reinterpret_cast<ECS::Handle<RigidBody>*>(pairHeader.actors[1]->userData);
        auto* rb2 = ecs.GetComponent(*comp2);

        auto& tp = pairs[0];
        if (tp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
            rb1->CollisionEntered(*rb2, CollisionInfo{nbPairs, pairs});
            rb2->CollisionEntered(*rb1, CollisionInfo{nbPairs, pairs});
        }
        if (tp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
            rb1->CollisionLeft(*rb2, CollisionInfo{nbPairs, pairs});
            rb2->CollisionLeft(*rb1, CollisionInfo{nbPairs, pairs});
        }
    }
} // TetraEngine