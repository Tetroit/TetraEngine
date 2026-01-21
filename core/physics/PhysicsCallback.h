#pragma once
#include "PxPhysicsAPI.h"
#include "../ecs/ECS.h"
#include "../utils/Action.h"

namespace TetraEngine {
    class RigidBody;
}

using namespace physx;
namespace TetraEngine {

    struct CollisionPointInfo {
        const glm::vec3 positions;
        const glm::vec3 normal;
        const float penetration;
        const glm::vec3 impulse;
    };
    struct CollisionInfo {
        PxU32 nContacts;
        std::vector<CollisionPointInfo> contacts;

        [[nodiscard]]
        CollisionPointInfo GetCollisionPoint(int id = 0) const {
            return contacts[id];
        }
    };

    struct TriggerInfo {
        uint nContacts;
        //add more info if necessary
    };
    class PhysicsCallback : public PxSimulationEventCallback {
        Action<const PxContactPairHeader &, const PxContactPair*, PxU32> onContactAction;
        std::unordered_map<ECS::Handle<RigidBody>, std::unordered_map<ECS::Handle<RigidBody>, CollisionPointInfo, ECS::HandleHasher<RigidBody>>,ECS::HandleHasher<RigidBody>> contactPairs;
    public:
        void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) override;
        void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

        void onConstraintBreak(PxConstraintInfo*, PxU32) override {}
        void onWake(PxActor**, PxU32) override {}
        void onSleep(PxActor**, PxU32) override {}
        void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) override {}
    };
}
