#pragma once
#include <utility>

#include "Transform.h"
#include "ecs/ECS.h"
#include "rendering/MeshRenderer.h"


namespace TetraEngine {
    class Scene;

    class GameObject {

    public:

        class Info {
        public:
            explicit Info(std::string name) : name(std::move(name)), scene(nullptr), isEnabled(true) {};
            std::string name;
            Scene* scene;
            bool isEnabled;
        };

    private:

        ECS::Entity entity;
        ECS::Handle<Transform> transform;
        ECS::Handle<Info> info;

    public:

        GameObject();
        explicit GameObject(const std::string &);
        ~GameObject();

        template<class T>
        T* GetComponent();
        template<class T, typename... Args>
        ECS::Handle<T> AddComponent(Args&&... args);

        Transform* GetTransform();
        ECS::Handle<Transform> GetTransformHandle();
        Info* GetInfo();
        std::string GetName();
        ECS::Entity GetEntity();
        void SetName(const std::string& name);
        void SetScene(Scene* scene);
        void SetEnabled(bool isEnabled);
        bool IsEnabled();

        template <class T>
        void OnComponentAdded(ECS::Handle<T> handle);

    };

    template<>
    void GameObject::OnComponentAdded<MeshRenderer>(ECS::Handle<MeshRenderer> handle);

}

#include "Core.h"

namespace TetraEngine
{
    template<class T>
    T* GameObject::GetComponent() {
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