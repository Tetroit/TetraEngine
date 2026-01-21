//
// Created by pchyo on 07/11/2025.
//

#include "PhysicsCallback.h"
#include "PhysXUtils.h"
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

            std::vector<CollisionPointInfo> collisionPoints;
            PxContactPairPoint contacts[8];
            PxU32 count = tp.extractContacts(contacts, 8);

            for (PxU32 j = 0; j < count; ++j)
            {
                CollisionPointInfo info{
                    PhysXUtils::PXToVec3(contacts[j].position),
                    PhysXUtils::PXToVec3(contacts[j].normal),
                    contacts[j].separation,
                    PhysXUtils::PXToVec3(contacts[j].impulse)
                };
                collisionPoints.push_back(info);
            }
            auto info = CollisionInfo{nbPairs, collisionPoints};
            rb1->CollisionEntered(*rb2, info);
            rb2->CollisionEntered(*rb1, info);
        }
        if (tp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
            std::vector<CollisionPointInfo> collisionPoints;
            PxContactPairPoint contacts[8];
            PxU32 count = tp.extractContacts(contacts, 8);

            for (PxU32 j = 0; j < count; ++j)
            {
                CollisionPointInfo info{
                    PhysXUtils::PXToVec3(contacts[j].position),
                    PhysXUtils::PXToVec3(contacts[j].normal),
                    contacts[j].separation,
                    PhysXUtils::PXToVec3(contacts[j].impulse)
                };
                collisionPoints.push_back(info);
            }
            auto info = CollisionInfo{nbPairs, collisionPoints};
            rb1->CollisionLeft(*rb2, info);
            rb2->CollisionLeft(*rb1, info);
        }
    }

    void PhysicsCallback::onTrigger(PxTriggerPair *pairs, PxU32 count) {
        TETRA_USE_MAIN_ECS
        auto* trigComp = reinterpret_cast<ECS::Handle<RigidBody>*>(pairs->triggerActor->userData);
        auto* trigRB = ecs.GetComponent(*trigComp);
        auto* collComp = reinterpret_cast<ECS::Handle<RigidBody>*>(pairs->otherActor->userData);
        auto* collRB = ecs.GetComponent(*collComp);

        auto& tp = pairs[0];
        if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
            auto info = TriggerInfo{count};
            trigRB->TriggerEntered(*collRB, info);
            collRB->TriggerEntered(*trigRB, info);
        }
        if (tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST) {
            auto info = TriggerInfo{count};
            if (trigRB != nullptr && collRB != nullptr) {
                trigRB->TriggerLeft(*collRB, info);
                collRB->TriggerLeft(*trigRB, info);
            }
        }
    }
} // TetraEngine