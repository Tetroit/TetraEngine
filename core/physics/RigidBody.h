#pragma once

#include "tetrapc.h"
#include <PxPhysicsAPI.h>
#include "../ecs/ECS.h"
using namespace physx;
namespace TetraEngine {

    class GameObjectInfo;
    class Transform;
    class Collider;
    struct CollisionInfo;

    struct DynamicRigidBodyData {
        PxVec3 linearVelocity = PxVec3(0.0f);
        PxVec3 angularVelocity = PxVec3(0.0f);
        PxVec3 linearAcceleration = PxVec3(0.0f);
        PxVec3 angularAcceleration = PxVec3(0.0f);
        float mass = 0.0f;
        PxVec3 inertiaTensor = PxVec3(0.0f);
    };
    class RigidBody {

        friend class PhysXInstance;
        friend class PhysicsScene;

        ECS::Handle<Transform> transformComp;
        ECS::Handle<GameObjectInfo> infoComp;
        PxRigidActor* rigidBody = nullptr;
        PxTransform previousTransform;
        PxTransform currentTransform;
        Collider* collider = nullptr;
        DynamicRigidBodyData dynamicData;

        bool isStatic = false;
        bool staticUpdated = false;
        bool isPlane = false;

        void ToStatic();
        void ToDynamic();

    public:

        static void ComponentCreate(RigidBody& rb, ECS::Entity entity, ECS::Handle<RigidBody> handle);
        explicit RigidBody(bool isStatic = false, bool isPlane = false);
        ~RigidBody();

        void ReadTransform();
        PxTransform GetTransform();
        void SetTransform(const PxTransform& transform);

        void SetCollider(Collider* collider);
        Collider* GetCollider();
        PxScene const* GetScene() const;
        void SetScene(PxScene* scene);

        float GetMass() const;
        void SetMass(float mass);

        [[nodiscard]] glm::vec3 GetPosition() const;
        void SetPosition(glm::vec3 pos);
        [[nodiscard]] glm::quat GetRotation() const;
        void SetRotation(glm::quat rot);
        [[nodiscard]] glm::vec3 GetLinearVelocity() const;
        void SetLinearVelocity(glm::vec3 vel);
        [[nodiscard]] glm::vec3 GetAngularVelocity() const;
        void SetAngularVelocity(glm::vec3 vel);

        [[nodiscard]] glm::vec3 GetLinearAcceleration() const;
        [[nodiscard]] glm::vec3 GetAngularAcceleration() const;

        void GetDynamicInfo(DynamicRigidBodyData& data) const;
        void SetDynamicInfo(const DynamicRigidBodyData& data);

        void AddAngular(glm::vec3 angularVelocity, PxForceMode::Enum mode = PxForceMode::eVELOCITY_CHANGE);
        void AddLinear(glm::vec3 linearVelocity, PxForceMode::Enum mode = PxForceMode::eVELOCITY_CHANGE);
        bool IsStatic() const;
        void SetStatic();
        void SetDynamic();

        void CollisionEntered(RigidBody& other, CollisionInfo info);
        void CollisionLeft(RigidBody& other, CollisionInfo info);
        Action<RigidBody&, CollisionInfo> OnCollisionEnter;
        Action<RigidBody&, CollisionInfo> OnCollisionLeave;
    };
} // TetraEngine