#pragma once

#include "../GameObject.h"
#include "../ecs/ECS.h"
#include "../../rendering/Shader.h"

using namespace TetraEngine;

class Ball {
    GameObject* gameObject;
    ECS::Handle<Transform> transform;
    ECS::Handle<RigidBody> rigidBody;
    ECS::Handle<MeshRenderer> renderer;
public:
    Ball(std::string name, int& model, Shader* shader,
        glm::vec3 position = glm::vec3(0,0,0),
        glm::vec3 rotation = glm::vec3(0,0,0),
        glm::vec3 scale = glm::vec3(1,1,1));

    ~Ball();

    Transform* GetTransform() const;
    RigidBody* GetRigidBody() const;
    MeshRenderer* GetRenderer() const;
    GameObject* GetGameObject() const;
};