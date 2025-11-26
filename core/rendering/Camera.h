#pragma once

#include "ViewProvider.h"
#include "../Transform.h"
#include "../ecs/ECS.h"

namespace TetraEngine {
    class Camera : public ViewProvider {
        ECS::Handle<Transform> transform;

        float fovY;
        float ratio;
        float nearPlane;
        float farPlane;

        glm::mat4 projectionView;

    public:
        static void ComponentCreate(Camera& camera, ECS::Entity entity, ECS::Handle<Camera> handle);

        void Enable() override;
        void Disable() override;
        explicit Camera(float pFovY = glm::radians(45.0f), float pRatio = 16.0f/9.0f, float pNearPlane = 0.1f, float pFarPlane = 100.0f);

        [[nodiscard]] float GetRatio() const override;
        void SetRatio(float pRatio) override;
        [[nodiscard]] float GetFOV() const;
        void SetFOV(float pRatio);
        [[nodiscard]] glm::mat4 GetViewMatrix() const override;
        glm::mat4 GetProjection() const override;
        void SetProjection(float pFovY, float pRatio, float pNearPlane, float pFarPlane) override;
        glm::vec3 GetPosition() override;
    };
} // TetraEngine
