#pragma once
#include "../../behaviours/UpdateManager.h"
#include "../../ecs/ECS.h"

namespace TetraEngine {
    class Camera;
    class Transform;
}

using namespace TetraEngine;
namespace GameDemo {
    class CameraConstraint : public IUpdatable {
        glm::vec3 viewDir = normalize(glm::vec3(0, -1, 1));
        float dist = 5.0f;
        ECS::Handle<Transform> camTransform;
        ECS::Handle<Transform> targetTransform;
    public:
        CameraConstraint(ECS::Handle<Transform> camTransform, ECS::Handle<Transform> targetTransform, glm::vec3 viewVec);
        void SetViewDir(glm::vec3 v);
        void SetDist(float d);

        [[nodiscard]] glm::vec3 GetViewDir() const;
        [[nodiscard]] float GetDist() const;

        void Update(float dt) override;
    };
} // GameDemo