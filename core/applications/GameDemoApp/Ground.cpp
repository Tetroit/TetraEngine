#include "Ground.h"

#include "../../physics/Collider.h"
#include "../../rendering/VertexData.h"

using namespace TetraEngine;
namespace GameDemo {
    Ground::Ground(Shader* shader,
    glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
        TETRA_USE_MAIN_ECS
        gameObject = new GameObject("Ground");
        std::shared_ptr<VertexData> vd = VertexData::GetPrefab(VD_RECTANGLE_Y);
        renderer = gameObject->AddComponent<MeshRenderer>(vd, shader);
        transform = gameObject->GetTransformHandle();
        gameObject->GetTransform()->SetLocalPosition(position);
        gameObject->GetTransform()->SetLocalRotation(glm::quat(rotation));
        gameObject->GetTransform()->SetLocalScale(scale);

        rigidBody = gameObject->AddComponent<RigidBody>(true, true);
    }

    Ground::~Ground() {
        delete gameObject;
    }

    RigidBody * Ground::GetRigidBody() const {
        TETRA_USE_MAIN_ECS
        return ecs.GetComponent(rigidBody);
    }

    MeshRenderer * Ground::GetRenderer() const {
        TETRA_USE_MAIN_ECS
        return ecs.GetComponent(renderer);
    }

    Transform * Ground::GetTransform() const {
        TETRA_USE_MAIN_ECS
        return ecs.GetComponent(transform);
    }

    GameObject * Ground::GetGameObject() const {
        return gameObject;
    }
} // GameDemo