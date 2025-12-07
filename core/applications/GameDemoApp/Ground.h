#pragma once

#include "../GameObject.h"
#include "../ecs/ECS.h"
#include "../../rendering/Shader.h"
using namespace TetraEngine;

namespace GameDemo {
    class Ground {
        GameObject* gameObject;
        ECS::Handle<Transform> transform;
        ECS::Handle<RigidBody> rigidBody;
        ECS::Handle<MeshRenderer> renderer;
    public:
        explicit Ground(Shader* shader,
            glm::vec3 position = glm::vec3(0,0,0),
            glm::vec3 rotation = glm::vec3(0,0,0),
            glm::vec3 scale = glm::vec3(1,1,1));

        ~Ground();

        [[nodiscard]] Transform* GetTransform() const;
        [[nodiscard]] RigidBody* GetRigidBody() const;
        [[nodiscard]] MeshRenderer* GetRenderer() const;
        [[nodiscard]] GameObject* GetGameObject() const;
    };
} // GameDemo