#include "PhysicsScene.h"

#include "PhysXUtils.h"
#include "RigidBody.h"
#include "PhysicsCallback.h"
#include "../ecs/ECS.h"
#include "../Core.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "glm/gtx/quaternion.hpp"

namespace TetraEngine {

    PhysicsScene::PhysicsScene() {
        TETRA_USE_MAIN_PHYSICS_INSTANCE

        auto gPhysics = physicsInstance->GetPhysics();
        auto gDispatcher = physicsInstance->GetDispatcher();

        PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
        sceneDesc.cpuDispatcher	= gDispatcher;
        sceneDesc.filterShader = FilterShader;
        sceneDesc.simulationEventCallback = new PhysicsCallback();


        scene = physicsInstance->GetPhysics()->createScene(sceneDesc);
    }

    PhysicsScene::~PhysicsScene() {
        PX_RELEASE(scene);
    }

    void PhysicsScene::AddObject(const RigidBody &rb) {
        scene->addActor(*rb.rigidBody);
    }

    void PhysicsScene::RemoveObject(const RigidBody &rb) {
        scene->removeActor(*rb.rigidBody);
    }
    void PhysicsScene::SetGravity(glm::vec3 g) {
        scene->setGravity(PhysXUtils::Vec3ToPX(g));
    }

    void PhysicsScene::Update(float dt) {
        if (isPaused) {
            return;
        }

        if (scene == nullptr) {
            LOG_ERR("PhysX scene is null");
            return;
        }

        TETRA_USE_MAIN_ECS

        accumulator += dt;
        float simDt = 1.0f / simulationRate;

        while (accumulator > simDt) {
            accumulator -= simDt;

            scene->simulate(simDt);
            scene->fetchResults(true);

            ecs.Foreach<RigidBody>([&](RigidBody& body) {
                if (body.GetScene() != scene) {
                    return;
                }
                body.ReadTransform();
            });
        }

        ecs.Foreach<RigidBody, Transform>([&](RigidBody& rb, Transform& t) {
            if (rb.GetScene() != scene) {
                return;
            }
            if (rb.isStatic && rb.staticUpdated) {
                return;
            }
            float fac = accumulator / simDt;
            const auto& tr2 = rb.currentTransform;
            const auto& tr1 = rb.previousTransform;
            glm::quat q1 = PhysXUtils::PXToQuat(tr1.q);
            glm::quat q2 = PhysXUtils::PXToQuat(tr2.q);
            glm::vec3 p1 = PhysXUtils::PXToVec3(tr1.p);
            glm::vec3 p2 = PhysXUtils::PXToVec3(tr2.p);
            glm::quat qLerp = glm::slerp(q1, q2, fac);
            glm::vec3 pLerp = p1 * (1 - fac) + p2 * fac;

            t.SetPosition(pLerp);
            t.SetRotation(qLerp);
            if (rb.isStatic) {
                rb.staticUpdated = true;
            }
        });
    }

    void PhysicsScene::PauseSimulation() {
        isPaused = true;
    }

    void PhysicsScene::ResumeSimulation() {
        isPaused = false;
    }

    PxFilterFlags PhysicsScene::FilterShader(PxFilterObjectAttributes attributes0,
                                             PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1,
                                             PxPairFlags &pairFlags, const void *constantBlock, PxU32 constantBlockSize)
    {
        pairFlags = PxPairFlag::eCONTACT_DEFAULT;
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;
        return PxFilterFlag::eDEFAULT;
    }
} // TetraEngine