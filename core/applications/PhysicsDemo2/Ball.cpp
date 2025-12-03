//
// Created by pchyo on 27/11/2025.
//

#include "Ball.h"

#include "../../physics/Collider.h"
#include "../../rendering/VertexData.h"
#include "../../utils/OBJParser.h"

Ball::Ball(std::string name, int& model, Shader* shader,
    glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    TETRA_USE_MAIN_ECS
    gameObject = new GameObject(name);
    std::shared_ptr<VertexData> VDSphere = VertexData::GetPrefab(model);
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

Transform * Ball::GetTransform() {
    TETRA_USE_MAIN_ECS
    return ecs.GetComponent(transform);
}

RigidBody * Ball::GetRigidBody() {
    TETRA_USE_MAIN_ECS
    return ecs.GetComponent(rigidBody);
}

MeshRenderer * Ball::GetRenderer() {
    TETRA_USE_MAIN_ECS
    return ecs.GetComponent(renderer);
}

GameObject * Ball::GetGameObject() {
    return gameObject;
}
