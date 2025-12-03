#pragma once

#include "tetrapc.h"
#include <PxPhysicsAPI.h>
#include "../ecs/ECS.h"

namespace TetraEngine {

    class RigidBody;
    class PhysicsScene {
        physx::PxScene* scene = nullptr;
        float accumulator = 0;
        const float simulationRate = 60.0f;
        bool isPaused = false;

    public:
        PhysicsScene();
        ~PhysicsScene();

        void AddObject(const RigidBody &rb);
        void RemoveObject(const RigidBody &rb);
        void SetGravity(glm::vec3 g);
        void Update(float dt);
        void PauseSimulation();
        void ResumeSimulation();

        static physx::PxFilterFlags FilterShader(
            physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
            physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
            physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);
    };
} // TetraEngine
