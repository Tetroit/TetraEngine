
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

        physx::PxPhysics* GetPhysics() const;
        physx::PxFoundation* GetFoundation() const;
        void SetActiveScene(PhysicsScene *scene);
        PhysicsScene* GetActiveScene() const;
        physx::PxMaterial* GetDefaultMaterial() const;

        physx::PxDefaultCpuDispatcher *GetDispatcher() const;

        ~PhysXInstance();
        void Initialise();
        void CleanUp();
    };
}
