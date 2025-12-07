#include "CameraConstraint.h"

#include "../../Core.h"

namespace GameDemo {
    CameraConstraint::CameraConstraint(
        ECS::Handle<Transform> camTransform,
        ECS::Handle<Transform> targetTransform,
        glm::vec3 viewVec) :
    viewDir(normalize(viewVec)),
    dist(length(viewVec)),
    camTransform(camTransform),
    targetTransform(targetTransform) {}

    void CameraConstraint::SetViewDir(glm::vec3 v) { viewDir = v; }
    void CameraConstraint::SetDist(float d) { dist = d; }

    glm::vec3 CameraConstraint::GetViewDir() const { return viewDir; }
    float CameraConstraint::GetDist() const { return dist; }

    void CameraConstraint::Update(float dt) {
        TETRA_USE_MAIN_ECS
        auto camTransformPtr = ecs.GetComponent(camTransform);
        auto targetTransformPtr = ecs.GetComponent(targetTransform);
        glm::vec3 pos = targetTransformPtr->GetPosition() - viewDir * dist;
        camTransformPtr->SetPosition(pos);

        glm::vec3 front = glm::normalize(viewDir);
        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0,1,0), front));
        glm::vec3 up = glm::cross(front, right);

        glm::mat3 rot;
        rot[0] = right;   // X axis
        rot[1] = up;   // Y axis
        rot[2] = front;   // Z axis

        camTransformPtr->SetRotation(glm::quat_cast(rot));
    }
} // GameDemo