#pragma once
#include "PxPhysicsAPI.h"
#include "../utils/Action.h"

using namespace physx;
namespace TetraEngine {

    struct CollisionInfo {
        PxU32 nContacts;
        const PxContactPair* contacts;

        [[nodiscard]]
        PxContactPairPoint* GetCollisionPoint(int id = 0) {
            PxContactPairPoint* cp = new PxContactPairPoint[8];
            int nPoints = contacts[0].extractContacts(cp, 8);
            if (nPoints <= id)
                return nullptr;
            auto res = new PxContactPairPoint(cp[id]);
            delete[] cp;
            return res;
        }
    };
    class PhysicsCallback : public PxSimulationEventCallback {
        Action<const PxContactPairHeader &, const PxContactPair*, PxU32> onContactAction;
    public:
        void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) override;
        void onTrigger(PxTriggerPair* pairs, PxU32 count) override {}
        void onConstraintBreak(PxConstraintInfo*, PxU32) override {}
        void onWake(PxActor**, PxU32) override {}
        void onSleep(PxActor**, PxU32) override {}
        void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) override {}
    };
}
