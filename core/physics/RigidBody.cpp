#include "RigidBody.h"
#include "Collider.h"
#include "PhysicsCallback.h"
#include "PhysX.h"
#include "PhysXUtils.h"
#include "../Core.h"
#include "../GameObject.h"

namespace TetraEngine {
    void RigidBody::ToStatic() {
        TETRA_USE_MAIN_PHYSICS
        GetDynamicInfo(dynamicData);
        auto scene = rigidBody->getScene();
        auto name = rigidBody->getName();
        auto userData = rigidBody->userData;
        rigidBody->detachShape(*collider->GetShape());
        PX_RELEASE(rigidBody);

        rigidBody = physics->createRigidStatic(currentTransform);
        isStatic = true;
        rigidBody->attachShape(*collider->GetShape());
        rigidBody->setName(name);
        rigidBody->userData = userData;
        scene->addActor(*rigidBody);
        staticUpdated = false;
    }
    void RigidBody::ToDynamic() {
        TETRA_USE_MAIN_PHYSICS
        rigidBody->detachShape(*collider->GetShape());
        auto scene = rigidBody->getScene();
        auto name = rigidBody->getName();
        auto userData = rigidBody->userData;
        PX_RELEASE(rigidBody);

        rigidBody = physics->createRigidDynamic(currentTransform);
        isStatic = false;
        rigidBody->attachShape(*collider->GetShape());
        rigidBody->setName(name);
        rigidBody->userData = userData;
        scene->addActor(*rigidBody);
        SetDynamicInfo(dynamicData);
    }

    void RigidBody::ComponentCreate(RigidBody &rb, ECS::Entity entity, ECS::Handle<RigidBody> handle) {
        TETRA_USE_MAIN_ECS
        TETRA_USE_MAIN_PHYSICS
        TETRA_USE_MAIN_PHYSICS_INSTANCE
        rb.transformComp = ecs.GetHandle<Transform>(entity);
        rb.infoComp = ecs.GetHandle<GameObjectInfo>(entity);
        auto t = ecs.GetComponent<Transform>(rb.transformComp);
        auto pos = t->GetPosition();
        auto rot = t->GetRotation();
        rb.currentTransform = PxTransform(PhysXUtils::Vec3ToPX(pos), PhysXUtils::QuatToPX(rot));
        rb.previousTransform = rb.currentTransform;
        if (rb.isStatic) {
            if (rb.isPlane)
                rb.rigidBody = PxCreatePlane(*physics, PxPlane(0, 1, 0, -pos.y), *physicsInstance->GetDefaultMaterial());
            else {
                rb.rigidBody = physics->createRigidStatic(rb.currentTransform);
            }
        }
        else
            rb.rigidBody = physics->createRigidDynamic(rb.currentTransform);
        auto info = ecs.GetComponent(rb.infoComp);
        if (rb.rigidBody == nullptr) {
            LOG_ERR("Failed to create rigid body");
            return;
        }

        rb.rigidBody->userData = reinterpret_cast<void*>(new ECS::Handle(handle));
    }

    ECS::Handle<RigidBody> RigidBody::GetHandleFromPxActor(const PxRigidActor &actor) {
        return *static_cast<ECS::Handle<RigidBody>*>(actor.userData);
    }

    RigidBody::RigidBody(bool isStatic, bool isPlane) : isStatic(isStatic), isPlane(isPlane) {
    }

    RigidBody::~RigidBody() {
    }

    PxTransform RigidBody::GetTransform() {
        return currentTransform;
    }

    void RigidBody::ReadTransform() {
        if (isStatic)
            return;

        if (rigidBody == nullptr) {
            LOG_ERR("RigidBody is null");
            return;
        }
        previousTransform = currentTransform;
        currentTransform = rigidBody->getGlobalPose();
    }

    void RigidBody::SetTransform(const PxTransform &transform) {
        previousTransform = currentTransform;
        currentTransform = transform;
    }

    void RigidBody::SetCollider(Collider *other) {

        if (other == collider) {
            return;
        }
        if (collider != nullptr) {
            rigidBody->detachShape(*collider->GetShape());
        }
        this->collider = other;
        rigidBody->attachShape(*collider->GetShape());
    }

    glm::vec3 RigidBody::GetPosition() const {
        return PhysXUtils::PXToVec3(currentTransform.p);
    }

    void RigidBody::SetPosition(glm::vec3 pos) {
        currentTransform.p = PhysXUtils::Vec3ToPX(pos);
        rigidBody->setGlobalPose(currentTransform);
    }

    glm::quat RigidBody::GetRotation() const {
        return PhysXUtils::PXToQuat(currentTransform.q);
    }

    void RigidBody::SetRotation(glm::quat rot) {
        currentTransform.q = PhysXUtils::QuatToPX(rot);
        rigidBody->setGlobalPose(currentTransform);
    }

    glm::vec3 RigidBody::GetLinearVelocity() const {
        if (isStatic) {
            return glm::vec3(0);
        }
        return PhysXUtils::PXToVec3(static_cast<PxRigidDynamic*>(rigidBody)->getLinearVelocity());
    }

    void RigidBody::SetLinearVelocity(glm::vec3 vel) {
        if (isStatic) {
            LOG("Couldn't set linear velocity, actor was static: " << Core::GetMainECS().GetComponent(infoComp)->name);
            return;
        }
        auto* rigidDynamic = static_cast<PxRigidDynamic*>(rigidBody);
        rigidDynamic->setLinearVelocity(PhysXUtils::Vec3ToPX(vel));
    }

    glm::vec3 RigidBody::GetAngularVelocity() const {
        if (isStatic) {
            return glm::vec3(0);
        }
        return PhysXUtils::PXToVec3(static_cast<PxRigidDynamic*>(rigidBody)->getAngularVelocity());
    }

    void RigidBody::SetAngularVelocity(glm::vec3 vel) {

        if (isStatic) {
            LOG("Couldn't set angular velocity, actor was static: " << Core::GetMainECS().GetComponent(infoComp)->name);
            return;
        }
        auto* rigidDynamic = static_cast<PxRigidDynamic*>(rigidBody);
        rigidDynamic->setAngularVelocity(PhysXUtils::Vec3ToPX(vel));
    }

    glm::vec3 RigidBody::GetLinearAcceleration() const {
        if (isStatic) {
            return glm::vec3(0);
        }
        return PhysXUtils::PXToVec3(static_cast<PxRigidDynamic*>(rigidBody)->getLinearAcceleration());
    }

    glm::vec3 RigidBody::GetAngularAcceleration() const {
        if (isStatic) {
            return glm::vec3(0);
        }
        return PhysXUtils::PXToVec3(static_cast<PxRigidDynamic*>(rigidBody)->getAngularAcceleration());
    }

    void RigidBody::GetDynamicInfo(DynamicRigidBodyData &data) const {
        if (isStatic)
            return;
        auto* rigidDynamic = static_cast<PxRigidDynamic*>(rigidBody);
        data.mass = rigidDynamic->getMass();
        data.inertiaTensor = rigidDynamic->getMassSpaceInertiaTensor();
        data.linearAcceleration = rigidDynamic->getLinearAcceleration();
        data.angularAcceleration = rigidDynamic->getAngularAcceleration();
        data.linearVelocity = rigidDynamic->getLinearVelocity();
        data.angularVelocity = rigidDynamic->getAngularVelocity();

    }

    void RigidBody::SetDynamicInfo(const DynamicRigidBodyData &data) {

        if (isStatic)
            return;
        auto* rigidDynamic = static_cast<PxRigidDynamic*>(rigidBody);
        rigidDynamic->setMass(data.mass);
        rigidDynamic->setMassSpaceInertiaTensor(data.inertiaTensor);
        rigidDynamic->setLinearVelocity(data.linearVelocity);
        rigidDynamic->setAngularVelocity(data.angularVelocity);
        rigidDynamic->addForce(data.linearAcceleration, PxForceMode::eACCELERATION);
        rigidDynamic->addTorque(data.angularAcceleration, PxForceMode::eACCELERATION);
    }

    void RigidBody::AddAngular(glm::vec3 angularVelocity, PxForceMode::Enum mode) {
        if (!isStatic) {
            auto* rigidDynamic = static_cast<PxRigidDynamic*>(rigidBody);
            rigidDynamic->addTorque(PhysXUtils::Vec3ToPX(angularVelocity), mode);
        }
        else {
            LOG_ERR("Static RigidBody " << Core::GetMainECS().GetComponent(infoComp)->name << " doesn't support momentum");
        }
    }
    void RigidBody::AddLinear(glm::vec3 linearVelocity, PxForceMode::Enum mode) {
        if (!isStatic) {
            auto* rigidDynamic = static_cast<PxRigidDynamic*>(rigidBody);
            rigidDynamic->addForce(PhysXUtils::Vec3ToPX(linearVelocity), PxForceMode::eIMPULSE);
        }
        else {
            LOG_ERR("Static RigidBody " << Core::GetMainECS().GetComponent(infoComp)->name << " doesn't support force");
        }
    }

    bool RigidBody::IsStatic() const {
        return isStatic;
    }

    void RigidBody::SetStatic() {
        if (isStatic)
            return;
        ToStatic();
    }

    void RigidBody::SetDynamic() {
        if (!isStatic)
            return;
        ToDynamic();
    }

    void RigidBody::CollisionEntered(RigidBody &other, CollisionInfo info) {
        TETRA_USE_MAIN_ECS
        auto* infoThis = ecs.GetComponent(infoComp);
        auto* infoOther = ecs.GetComponent(other.infoComp);
        //LOG(infoThis->name << " entered collision with " << infoOther->name);
        OnCollisionEnter.Call(other, info);
    }
    void RigidBody::CollisionLeft(RigidBody &other, CollisionInfo info) {
        TETRA_USE_MAIN_ECS
        auto* infoThis = ecs.GetComponent(infoComp);
        auto* infoOther = ecs.GetComponent(other.infoComp);
        //LOG(infoThis->name << " left collision with " << infoOther->name);
        OnCollisionEnter.Call(other, info);
    }

    Collider * RigidBody::GetCollider() {
        return collider;
    }

    PxScene const * RigidBody::GetScene() const {
        return rigidBody->getScene();
    }

    void RigidBody::SetScene(PxScene *scene) {
        scene->addActor(*rigidBody);
    }

    float RigidBody::GetMass() const {
        if (!isStatic) {
            auto* rigidDynamic = static_cast<PxRigidDynamic*>(rigidBody);
            return rigidDynamic->getMass();
        }
        return 0.0f;
    }

    void RigidBody::SetMass(float mass) {
        if (!isStatic) {
            auto* rigidDynamic = static_cast<PxRigidDynamic*>(rigidBody);
            rigidDynamic->setMass(mass);
        }
    }
} // TetraEngine