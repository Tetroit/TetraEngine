#include "tetrapc.h"

#include "GameObject_new.h"

#include "Core.h"
#include "rendering/Scene.h"

namespace TetraEngine {
    GameObject::GameObject() {
        entity = Core::GetMainECS().CreateEntity();
        transform = Core::GetMainECS().CreateComponent<Transform>(entity);
        info = Core::GetMainECS().CreateComponent<Info>(entity, "New Object");
    }

    GameObject::GameObject(const std::string& name) {
        entity = Core::GetMainECS().CreateEntity();
        transform = Core::GetMainECS().CreateComponent<Transform>(entity);
        info = Core::GetMainECS().CreateComponent<Info>(entity, name);
    }

    GameObject::~GameObject() {
        Core::GetMainECS().RemoveEntity(entity);
    }

    Transform* GameObject::GetTransform() {
        return Core::GetMainECS().GetComponent<Transform>(transform);
    }

    ECS::Handle<Transform> GameObject::GetTransformHandle() {
        return transform;
    }

    GameObject::Info* GameObject::GetInfo() {
        return Core::GetMainECS().GetComponent(info);
    }

    std::string GameObject::GetName() {
        return GetComponent<Info>()->name;
    }

    ECS::Entity GameObject::GetEntity() {
        return entity;
    }

    void GameObject::SetName(const std::string &name) {
        GetInfo()->name = name;
    }

    void GameObject::SetScene(Scene *sc) {
        GetInfo()->scene = sc;
    }

    void GameObject::SetEnabled(bool isEnabled) {
        GetInfo()->isEnabled = isEnabled;
    }

    bool GameObject::IsEnabled() {
        return GetInfo()->isEnabled;
    }

    template<>
    void GameObject::OnComponentAdded<MeshRenderer>(ECS::Handle<MeshRenderer> handle) {
        auto infoRef = GetInfo();
        auto scene = infoRef->scene;
        if (!handle.Valid()) {
            LOG_ERR("Handle is invalid");
            return;
        }
        if (scene == Scene::currentScene) {
            scene->RegisterShader(Core::GetMainECS().GetComponent(handle)->shader);
        }
    }
} // TetraEngine