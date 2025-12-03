#include "Ground.h"

#include "../../physics/Collider.h"
#include "../../rendering/VertexData.h"

using namespace TetraEngine;
namespace GameDemo {
    Ground::Ground(Shader* shader,
    glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
        TETRA_USE_MAIN_ECS
        gameObject = new GameObject("Ground");
        std::shared_ptr<VertexData> VDSphere = VertexData::GetPrefab(VD_RECTANGLE_Y);
        renderer = gameObject->AddComponent<MeshRenderer>(VDSphere, shader);
        transform = gameObject->GetTransformHandle();
        gameObject->GetTransform()->SetLocalPosition(position);
        gameObject->GetTransform()->SetLocalRotation(glm::quat(rotation));
        gameObject->GetTransform()->SetLocalScale(scale);

        rigidBody = gameObject->AddComponent<RigidBody>();
        auto RBBall = ecs.GetComponent(rigidBody);
        auto* CollBall = new Collider();
        CollBall->AddSphere(gameObject->GetTransform()->GetLocalScale().x);
        RBBall->SetCollider(CollBall);
    }

    Ground::~Ground() {
        delete gameObject;
    }
} // GameDemo