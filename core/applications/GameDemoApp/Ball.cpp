#include "Ball.h"

#include "../../physics/Collider.h"
#include "../../rendering/VertexData.h"
#include "../../utils/OBJParser.h"

using namespace GameDemo;

void Ball::SetVD(int vd) {
    vdId = vd;
}

Ball::Ball(std::string name, Shader* shader,
           glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    TETRA_USE_MAIN_ECS
    gameObject = new GameObject(name);
    std::shared_ptr<VertexData> VDSphere = VertexData::GetPrefab(vdId);
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

Ball::~Ball() {
    delete gameObject;
}

Transform * Ball::GetTransform() const {
    TETRA_USE_MAIN_ECS
    return ecs.GetComponent(transform);
}

RigidBody * Ball::GetRigidBody() const {
    TETRA_USE_MAIN_ECS
    return ecs.GetComponent(rigidBody);
}

MeshRenderer * Ball::GetRenderer() const {
    TETRA_USE_MAIN_ECS
    return ecs.GetComponent(renderer);
}

GameObject * Ball::GetGameObject() const {
    return gameObject;
}

ECS::Handle<Transform> Ball::GetTransformHandle() const {
    return transform;
}

ECS::Handle<RigidBody> Ball::GetRigidBodyHandle() const {
    return rigidBody;
}

ECS::Handle<MeshRenderer> Ball::GetRendererHandle() const {
    return renderer;
}
