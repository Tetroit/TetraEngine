#include "Camera.h"

#include "../Core.h"

namespace TetraEngine {
    void Camera::ComponentCreate(Camera &camera, ECS::Entity entity, ECS::Handle<Camera> handle) {
        TETRA_USE_MAIN_ECS
        camera.transform = ecs.GetHandle<Transform>(entity);
    }

    void Camera::Enable() {

    }

    void Camera::Disable() {

    }

    Camera::Camera(float pFovY, float pRatio, float pNearPlane, float pFarPlane) :
    fovY(pFovY),
    ratio(pRatio),
    nearPlane(pNearPlane),
    farPlane(pFarPlane),
    projectionView(glm::perspective(fovY, ratio, nearPlane, farPlane))
    {}

    glm::mat4 Camera::GetViewMatrix() const
    {
        TETRA_USE_MAIN_ECS
        auto transformPtr = ecs.GetComponent(transform);
        return glm::lookAt(
            transformPtr->GetPosition(),
            transformPtr->GetPosition() + transformPtr->TransformDirectionToWorld(glm::vec3(0,0,1)),
            transformPtr->TransformDirectionToWorld(glm::vec3(0,1,0)));
    }

    glm::mat4 Camera::GetProjection() const {
        return projectionView;
    }

    float Camera::GetRatio() const {
        return ratio;
    }

    void Camera::SetRatio(float pRatio) {
        ratio = pRatio;
        projectionView = glm::perspective(fovY, ratio, nearPlane, farPlane);
    }

    float Camera::GetFOV() const {
        return fovY;
    }

    void Camera::SetFOV(float pFovY) {
        fovY = pFovY;
        projectionView = glm::perspective(fovY, ratio, nearPlane, farPlane);
    }

    void Camera::SetProjection(float pFovY, float pRatio, float pNearPlane, float pFarPlane) {
        fovY = pFovY;
        ratio = pRatio;
        nearPlane = pNearPlane;
        farPlane = pFarPlane;
        projectionView = glm::perspective(fovY, ratio, nearPlane, farPlane);
    }

    glm::vec3 Camera::GetPosition() {
        TETRA_USE_MAIN_ECS
        auto transformPtr = ecs.GetComponent(transform);
        return transformPtr->GetPosition();
    }
} // TetraEngine