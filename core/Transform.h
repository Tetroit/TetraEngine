#pragma once

#include "tetrapc.h"
#include "ecs/ECS.h"
#include "utils/Action.h"


namespace TetraEngine {
    class Transform {

        friend class ECS::ECS;
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        glm::vec3 g_position;
        glm::quat g_rotation;
        glm::vec3 g_scale;

        glm::mat4 localMatrix;
        glm::mat4 globalMatrix;
        ECS::Handle<Transform> parent;
        std::vector<ECS::Handle<Transform>> children;
        ECS::Handle<Transform> self;

        bool isDirty;

        public:

        static void ComponentCreate(Transform& transform, ECS::Entity entity, ECS::Handle<Transform> handle);
        static void BindEvents();

        Transform();

        void MarkDirty();
        [[nodiscard]]
        bool IsDirty() const;

        [[deprecated]]
        void SetParent(ECS::Handle<Transform> transform); //work in progress
        static void SetParent(ECS::Handle<Transform> transform, ECS::Handle<Transform> parent);

        //Args => new parent, sender
        Action<ECS::Handle<Transform>&, ECS::Handle<Transform>&> OnSetParent;

        [[nodiscard]]
        Transform* GetParent() const;
        [[nodiscard]]
        ECS::Handle<Transform> GetParentHandle() const;

        [[nodiscard]]
        Transform* GetChild(uint id) const;
        [[nodiscard]]
        std::ranges::subrange<std::vector<ECS::Handle<Transform>>::iterator> GetChildren();
        [[nodiscard]]
        std::ranges::subrange<std::vector<ECS::Handle<Transform>>::const_iterator> GetChildrenConst() const;

        [[nodiscard]]
        glm::mat4 GetLocalMatrix();
        [[nodiscard]]
        glm::mat4 GetGlobalMatrix();
        [[nodiscard]]
        glm::mat4 GetParentMatrix();

        void Recalculate();

        void LocalTranslate(glm::vec3 pos);
        void LocalRotate(glm::quat rot);
        void LocalScale(glm::vec3 sc);

        void GlobalTranslate(glm::vec3 pos);
        void GlobalRotate(glm::quat rot);
        void GlobalScale(glm::vec3 sc);

        glm::vec3 GetPosition();
        glm::quat GetRotation();
        glm::vec3 GetScale();

        glm::vec3 GetLocalPosition();
        glm::quat GetLocalRotation();
        glm::vec3 GetLocalScale();

        void SetPosition(glm::vec3 pos);
        void SetRotation(glm::quat rot);
        void SetScale(glm::vec3 scale);

        void SetLocalPosition(glm::vec3 pos);
        void SetLocalRotation(glm::quat rot);
        void SetLocalScale(glm::vec3 scale);

        glm::vec3 TransformPointToWorld(glm::vec3 pos);
        glm::vec3 TransformPointToLocal(glm::vec3 pos);
        glm::vec3 TransformDirectionToWorld(glm::vec3 pos);
        glm::vec3 TransformDirectionToLocal(glm::vec3 pos);
    };
}
