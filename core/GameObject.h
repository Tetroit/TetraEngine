#pragma once
#include <utility>

#include "Transform.h"
#include "ecs/ECS.h"
#include "rendering/MeshRenderer.h"

namespace TetraEngine {

    class Scene;
    class RigidBody;

    class GameObjectInfo {
    public:
        explicit GameObjectInfo(std::string name, ECS::Entity entity);
        std::string name;
        Scene* scene;
        bool isEnabled;
        ECS::Entity entity;
    };

    class GameObject {

    public:

    private:

        ECS::Entity entity;
        ECS::Handle<Transform> transform;
        ECS::Handle<GameObjectInfo> info;

    public:

        GameObject();
        explicit GameObject(const std::string &);
        ~GameObject();

        template<class T>
        T* GetComponent() const;
        template<class T, typename... Args>
        ECS::Handle<T> AddComponent(Args&&... args);

        Transform* GetTransform() const;
        ECS::Handle<Transform> GetTransformHandle() const;
        GameObjectInfo* GetInfo() const;
        ECS::Handle<GameObjectInfo> GetInfoHandle() const;

        std::string GetName() const;
        ECS::Entity GetEntity() const;
        void SetName(const std::string& name) const;
        void SetScene(Scene* scene) const;
        void SetEnabled(bool isEnabled) const;
        bool IsEnabled() const;
        void Destroy();

        template <class T>
        void OnComponentAdded(ECS::Handle<T> handle);

    };

    template<>
    void GameObject::OnComponentAdded<MeshRenderer>(ECS::Handle<MeshRenderer> handle);

    template<>
    void GameObject::OnComponentAdded<RigidBody>(ECS::Handle<RigidBody> handle);

}

#include "Core.h"
#include "physics/RigidBody.h"

namespace TetraEngine
{
    template<class T>
    T* GameObject::GetComponent() const {
        return Core::GetMainECS().GetComponent<T>(entity);
    }

    template<class T, typename... Args>
    ECS::Handle<T> GameObject::AddComponent(Args&&... args) {
        auto handle = Core::GetMainECS().CreateComponent<T>(entity, std::forward<Args>(args)...);
        OnComponentAdded<T>(handle);
        return handle;
    }

    template<class T>
    void GameObject::OnComponentAdded(ECS::Handle<T> handle) {
    }
} // TetraEngine