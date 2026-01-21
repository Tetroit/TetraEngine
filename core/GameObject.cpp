#include "tetrapc.h"

#include "GameObject.h"

#include "Core.h"
#include "physics/PhysicsScene.h"
#include "rendering/Scene.h"
#include "physics/RigidBody.h"

namespace TetraEngine {
    GameObjectInfo::GameObjectInfo(std::string name, ECS::Entity entity): name(std::move(name)), scene(nullptr), isEnabled(true), entity(entity) {}

    GameObject::GameObject() {
        entity = Core::GetMainECS().CreateEntity();
        transform = Core::GetMainECS().CreateComponent<Transform>(entity);

        info = Core::GetMainECS().CreateComponent<GameObjectInfo>(entity, "New Object", entity);
    }

    GameObject::GameObject(const std::string& name) {
        entity = Core::GetMainECS().CreateEntity();
        transform = Core::GetMainECS().CreateComponent<Transform>(entity);
        info = Core::GetMainECS().CreateComponent<GameObjectInfo>(entity, name, entity);
    }

    GameObject::~GameObject() {
        Core::GetMainECS().RemoveEntity(entity);
    }

    Action<GameObject*>* GameObject::GetDestroyAction() {
        return &onDestroy;
    }

    Transform* GameObject::GetTransform() const {
        return Core::GetMainECS().GetComponent<Transform>(transform);
    }

    ECS::Handle<Transform> GameObject::GetTransformHandle() const {
        return transform;
    }

    GameObjectInfo* GameObject::GetInfo() const {
        return Core::GetMainECS().GetComponent(info);
    }

    ECS::Handle<GameObjectInfo> GameObject::GetInfoHandle() const {
        return info;
    }

    std::string GameObject::GetName() const {
        return GetComponent<GameObjectInfo>()->name;
    }

    ECS::Entity GameObject::GetEntity() const {
        return entity;
    }

    void GameObject::SetName(const std::string &name) const {
        GetInfo()->name = name;
    }

    void GameObject::SetScene(Scene *scene) const {
        GetInfo()->scene = scene;
    }

    void GameObject::SetEnabled(bool isEnabled) const {
        GetInfo()->isEnabled = isEnabled;
    }

    bool GameObject::IsEnabled() const {
        return GetInfo()->isEnabled;
    }

    void GameObject::Destroy() {
        Core::destroyManager->Push(this);
        onDestroy.Call(this);
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

    template<>
    void GameObject::OnComponentAdded<RigidBody>(ECS::Handle<RigidBody> handle) {
        auto infoRef = GetInfo();
        auto scene = infoRef->scene;
        if (!handle.Valid()) {
            LOG_ERR("Handle is invalid");
            return;
        }
        if (scene == Scene::currentScene) {
            scene->GetPhysicsScene()->AddObject(*Core::GetMainECS().GetComponent(handle));
        }
    }
} // TetraEngine