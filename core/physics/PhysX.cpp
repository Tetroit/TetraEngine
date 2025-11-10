//
// Created by pchyo on 27/10/2025.
//

#include "PhysX.h"
#include "RigidBody.h"
#include "PhysicsScene.h"
#include "../ecs/ECS.h"
#include "../Core.h"
#include "PhysXUtils.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "glm/gtx/quaternion.hpp"

using namespace TetraEngine;

PhysXInstance::~PhysXInstance() {
    CleanUp();
}

void PhysXInstance::Initialise() {
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    if(!gFoundation)
        LOG_ERR("PxCreateFoundation failed!");
    gPvd = PxCreatePvd(*gFoundation);
	//physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    //gPvd->connect(*transport,physx::PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);

    gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);

    defaultMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
}

void PhysXInstance::CleanUp() {
    PX_RELEASE(gDispatcher);
    PX_RELEASE(gPhysics);

    if(gPvd)
    {
        physx::PxPvdTransport* transport = gPvd->getTransport();
        PX_RELEASE(gPvd);
        PX_RELEASE(transport);
    }
    PX_RELEASE(gFoundation);
}

physx::PxPhysics * PhysXInstance::GetPhysics() const {
    return gPhysics;
}

physx::PxFoundation * PhysXInstance::GetFoundation() const {
    return gFoundation;
}

void PhysXInstance::SetActiveScene(PhysicsScene* scene) {
    activeScene = scene;
}

PhysicsScene* PhysXInstance::GetActiveScene() const {
    return activeScene;
}

physx::PxMaterial * PhysXInstance::GetDefaultMaterial() const {
    return defaultMaterial;
}

physx::PxDefaultCpuDispatcher * PhysXInstance::GetDispatcher() const {
    return gDispatcher;
}
