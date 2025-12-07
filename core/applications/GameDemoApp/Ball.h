#pragma once

#include "../GameObject.h"
#include "../ecs/ECS.h"
#include "../../rendering/Shader.h"

using namespace TetraEngine;
namespace GameDemo{
    class Ball {
        inline static int vdId = -1;
        GameObject* gameObject;
        ECS::Handle<Transform> transform;
        ECS::Handle<RigidBody> rigidBody;
        ECS::Handle<MeshRenderer> renderer;
    public:
        static void SetVD(int vd);
        Ball(std::string name, Shader* shader,
            glm::vec3 position = glm::vec3(0,0,0),
            glm::vec3 rotation = glm::vec3(0,0,0),
            glm::vec3 scale = glm::vec3(1,1,1));

        ~Ball();

        [[nodiscard]] Transform* GetTransform() const;
        [[nodiscard]] RigidBody* GetRigidBody() const;
        [[nodiscard]] MeshRenderer* GetRenderer() const;
        [[nodiscard]] GameObject* GetGameObject() const;

        [[nodiscard]] ECS::Handle<Transform> GetTransformHandle() const;
        [[nodiscard]] ECS::Handle<RigidBody> GetRigidBodyHandle() const;
        [[nodiscard]] ECS::Handle<MeshRenderer> GetRendererHandle() const;
    };
}