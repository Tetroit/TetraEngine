#pragma once

#include "tetrapc.h"
#include "ecs/ECS.h"


namespace TetraEngine {
    class Transform {

        friend class ECS::ECS;

        glm::mat4 localMatrix;
        glm::mat4 globalMatrix;
        ECS::Handle<Transform> parent;
        std::vector<ECS::Handle<Transform>> children;

        bool isDirty;

        public:
        Transform();

        void MarkDirty();
        [[nodiscard]]
        bool IsDirty() const;

        void SetParent(ECS::Handle<Transform> transform);
        static void SetParent(ECS::Handle<Transform> transform, ECS::Handle<Transform> parent);

        [[nodiscard]]
        Transform* GetParent() const;
        [[nodiscard]]
        Transform* GetChild(uint id) const;
        [[nodiscard]]
        glm::mat4 GetLocalMatrix() const;
        [[nodiscard]]
        glm::mat4 GetGlobalMatrix() const;

        void Recalculate();
        void RecalculateGlobalMatrix();
        void RecalculateWith(glm::mat4 m);

        void LocalTranslate(glm::vec3 pos);
        void LocalRotate(glm::quat rot);
        void LocalScale(glm::vec3 pos);

        void GlobalTranslate(glm::vec3 pos);
        void GlobalRotate(glm::quat rot);
        void GlobalScale(glm::vec3 scale);

        glm::vec3 GetPosition();
        glm::quat GetRotation();
        glm::vec3 GetScale();

        glm::vec3 GetLocalPosition();
        glm::quat GetLocalRotation();
        glm::vec3 GetLocalScale();
    };
}
