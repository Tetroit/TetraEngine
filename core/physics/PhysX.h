
#pragma once

#define PX_PHYSX_STATIC_LIB

#include "tetrapc.h"
#include <PxPhysicsAPI.h>

namespace TetraEngine {

    class RigidBody;
    class PhysicsScene;

    class PhysXInstance {

        physx::PxDefaultAllocator		gAllocator;
        physx::PxDefaultErrorCallback	gErrorCallback;
        physx::PxFoundation*			gFoundation = nullptr;
        physx::PxPhysics*				gPhysics = nullptr;
        physx::PxDefaultCpuDispatcher*	gDispatcher = nullptr;
        PhysicsScene*					activeScene = nullptr;
        physx::PxPvd*                   gPvd = nullptr;
        physx::PxMaterial*		        defaultMaterial = nullptr;

    public:

        [[nodiscard]] physx::PxPhysics* GetPhysics() const;
        [[nodiscard]] physx::PxFoundation* GetFoundation() const;
        void SetActiveScene(PhysicsScene *scene);
        [[nodiscard]] PhysicsScene* GetActiveScene() const;
        [[nodiscard]] physx::PxMaterial* GetDefaultMaterial() const;
        [[nodiscard]] physx::PxDefaultCpuDispatcher *GetDispatcher() const;

        ~PhysXInstance();
        void Initialise();
        void CleanUp();
    };
}
