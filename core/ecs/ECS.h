#pragma once
#include "Storage.h"
#include "../utils/Action.h"


#define TETRA_COMPONENT_EVENT_LISTENER_PARAMS(Type) Type& component, ECS::Entity entity, ECS::Handle<Type> handle

namespace TetraEngine::ECS {
    class ECS {

        std::unordered_map<size_t, std::unique_ptr<IStorage>> storages;
        std::vector<uint> entityGenerations;
        std::vector<entityID> freeIDs;

        template<class T>
        Storage<T>* CreateStorage();
        template<class T>
        Storage<T>* GetOrCreateStorage();

        bool ValidateEntitySilent(Entity& entity);

    public:

        template<class T>
        Action<T&, Entity, Handle<T>>& OnComponentCreated();
        template<class T>
        Action<T&, Entity, Handle<T>>& OnComponentDestroyed();

        template<class T>
        Storage<T>* GetStorage() const;

        template<class T>
        bool HasStorage() const {
            return storages.contains(typeid(T).hash_code());
        }

        Entity CreateEntity();
        bool ValidateEntity(const Entity& entity) const;
        void RemoveEntity(Entity& entity);
        bool TryRemoveEntity(Entity& entity);
        std::vector<std::pair<uint, TypeErasedHandle>> GetAllEntityComponents(const Entity& entity);

        template<class T, typename... Args>
        Handle<T> CreateComponent(Entity entity, Args&&... args);

        template<class T>
        T* GetComponent(const Entity& entity);
        template<class T>
        T* GetComponent(Handle<T> handle);
        template<class T, class U>
        T* GetRelatedComponent(Handle<U> component);
        template<class T, class U>
        Handle<T> GetRelatedComponentHandle(Handle<U> component);
        template<class T>
        Handle<T> GetHandle(Entity entity) const;

        template<class T>
        bool HasComponent(Entity entity);

        template<class T>
        void RemoveComponent(Handle<T>& handle);
        template<class T>
        bool TryRemoveComponent(Handle<T>& handle);
        template<class T>
        void RemoveComponent(Entity& entity);
        template<class T>
        bool TryRemoveComponent(Entity& entity);

        template<class T, typename FN>
        void Foreach(FN&& fn);
        template<class T1, class T2, typename FN>
        void Foreach(FN&& fn);
        template<class T1, class T2, class T3, typename FN>
        void Foreach(FN&& fn);
        template<class T1, class T2, class T3, class T4, typename FN>
        void Foreach(FN&& fn);
    };

    template<class T>
    Storage<T>* ECS::CreateStorage() {
        auto id = typeid(T).hash_code();
        storages[id] = std::make_unique<Storage<T>>();
        storages.at(id)->SetEntityResolutionMethod([&](entityID eid){return Entity(eid, entityGenerations[eid]);});
        return static_cast<Storage<T>*>(storages[id].get());
    }

    template<class T>
    Storage<T>* ECS::GetOrCreateStorage() {
        auto id = typeid(T).hash_code();
        if (auto loc = storages.find(id); loc == storages.end()) {
            return CreateStorage<T>();
        }
        return static_cast<Storage<T>*>(storages[id].get());
    }

    template<class T>
    Action<T&, Entity, Handle<T>>& ECS::OnComponentCreated()  {
        Storage<T>* storage = GetOrCreateStorage<T>();
        return storage->GetComponentAddedAction();
    }

    template<class T>
    Action<T&, Entity, Handle<T>>& ECS::OnComponentDestroyed() {
        Storage<T>* storage = GetOrCreateStorage<T>();
        return storage->GetComponentRemovedAction();
    }

    template<class T>
    Storage<T> * ECS::GetStorage() const {
        auto id = typeid(T).hash_code();
        if (auto loc = storages.find(id); loc == storages.end()) {
            //LOG_ERR("GetStorage: Storage " << typeid(T).name() << " was not found");
            return nullptr;
        }
        return static_cast<Storage<T>*>(storages.at(id).get());
    }

    template<class T, typename ... Args>
    Handle<T> ECS::CreateComponent(Entity entity, Args &&...args) {
        if (!ValidateEntity(entity)) {
            return Handle<T>::CreateInvalid();
        }
        Storage<T>* storage = GetOrCreateStorage<T>();
        if (HasComponent<T>(entity)) {
            LOG_ERR("Component already exists");
            return Handle<T>::CreateInvalid();
        }
        auto handle = storage->Create(entity.ID, std::forward<Args>(args)...);
        if (!storage->GetComponentAddedAction().IsEmpty()) {
            storage->GetComponentAddedAction().Call(*storage->Get(handle), entity, handle);
        }
        return handle;
    }

    template<class T>
    T* ECS::GetComponent(const Entity& entity) {
        auto storage = GetStorage<T>();
        if (storage == nullptr) {
            LOG_ERR("GetComponent(entity): Storage " << typeid(T).name() << " was not found");
            return nullptr;
        }
        return storage->GetByEntity(entity.ID);
    }

    template<class T>
    T* ECS::GetComponent(Handle<T> handle) {
        auto storage = GetStorage<T>();
        if (storage == nullptr) {
            LOG_ERR("GetComponent(handle): Storage " << typeid(T).name() << " was not found");
            return nullptr;
        }
        return storage->Get(handle);
    }

    template<class T, class U>
    T* ECS::GetRelatedComponent(Handle<U> component) {

        auto handle = GetRelatedComponentHandle<T,U>(component);
        if (!handle.Valid())
            return nullptr;

        auto dstStorage = GetStorage<T>();
        return dstStorage->Get(handle);
    }

    template<class T, class U>
    Handle<T> ECS::GetRelatedComponentHandle(Handle<U> component) {
        auto srcStorage = GetStorage<U>();
        auto dstStorage = GetStorage<T>();
        if (srcStorage == nullptr) {
            LOG_ERR("Couldn't find storage of type" + std::string(typeid(U).name()));
            return Handle<T>::CreateInvalid();
        }
        if (srcStorage == nullptr) {
            LOG_ERR("Couldn't find storage of type" + std::string(typeid(T).name()));
            return Handle<T>::CreateInvalid();
        }

        entityID entity = srcStorage->GetOwner(component);
        return dstStorage->GetHandle(entity);
    }

    template<class T>
    Handle<T> ECS::GetHandle(Entity entity) const {
        if (!ValidateEntity(entity)) {
            return Handle<T>::CreateInvalid();
        }
        auto storage = GetStorage<T>();
        if (storage == nullptr) {
            LOG_ERR("GetHandle: Storage " << typeid(T).name() << " was not found");
            return Handle<T>::CreateInvalid();
        }
        return storage->GetHandle(entity.ID);
    }

    template<class T>
    bool ECS::HasComponent(Entity entity) {
        if (!ValidateEntity(entity)) {
            return false;
        }
        return GetStorage<T>()->HasEntity(entity.ID);
    }

    template<class T>
    void ECS::RemoveComponent(Handle<T> &handle) {
        GetStorage<T>() -> Remove(handle);
    }

    template<class T>
    bool ECS::TryRemoveComponent(Handle<T> &handle) {
        return GetStorage<T>()->TryRemove(handle.ID);
    }

    template<class T>
    void ECS::RemoveComponent(Entity &entity) {
        if (!ValidateEntity(entity)) {
            return;
        }
        GetStorage<T>() -> RemoveFromEntity(entity.ID);
    }

    template<class T>
    bool ECS::TryRemoveComponent(Entity &entity) {
        if (!ValidateEntitySilent(entity)) {
            return false;
        }
        return GetStorage<T>() -> TryRemoveFromEntity(entity.ID);
    }

    template<class T, typename FN>
    void ECS::Foreach(FN &&fn) {
        auto storage = GetStorage<T>();
        if (storage == nullptr) {
            return;
        }
        storage->Foreach(std::forward<FN>(fn));
    }

    template<class T1, class T2, typename FN>
    void ECS::Foreach(FN &&fn) {
        auto storage1 = GetStorage<T1>();
        if (storage1 == nullptr) {
            return;
        }
        auto storage2 = GetStorage<T2>();
        if (storage2 == nullptr) {
            return;
        }

        auto& st1 = *storage1;
        auto& st2 = *storage2;
        auto entList = st1.GetEntities();
        for (auto ent : entList ) {
            auto& comp1 = st1.GetByEntityRef(ent);
            auto comp2 = st2.GetByEntity(ent);
            if (!comp2) continue;
            fn(comp1, *comp2);
        }
    }

    template<class T1, class T2, class T3, typename FN>
    void ECS::Foreach(FN &&fn) {
        auto storage1 = GetStorage<T1>();
        if (storage1 == nullptr) {
            return;
        }
        auto storage2 = GetStorage<T2>();
        if (storage2 == nullptr) {
            return;
        }
        auto storage3 = GetStorage<T3>();
        if (storage3 == nullptr) {
            return;
        }

        auto& st1 = *storage1;
        auto& st2 = *storage2;
        auto& st3 = *storage3;
        auto entList = st1.GetEntities();
        for (auto ent : entList ) {
            auto& comp1 = st1.GetByEntityRef(ent);
            auto comp2 = st2.GetByEntity(ent);
            if (!comp2) continue;
            auto comp3 = st3.GetByEntity(ent);
            if (!comp3) continue;
            fn(comp1, *comp2, *comp3);
        }
    }

    template<class T1, class T2, class T3, class T4, typename FN>
    void ECS::Foreach(FN &&fn) {
        auto storage1 = GetStorage<T1>();
        if (storage1 == nullptr) {
            return;
        }
        auto storage2 = GetStorage<T2>();
        if (storage2 == nullptr) {
            return;
        }
        auto storage3 = GetStorage<T3>();
        if (storage3 == nullptr) {
            return;
        }
        auto storage4 = GetStorage<T4>();
        if (storage4 == nullptr) {
            return;
        }

        auto& st1 = *storage1;
        auto& st2 = *storage2;
        auto& st3 = *storage3;
        auto& st4 = *storage4;
        auto entList = st1.GetEntities();
        for (auto ent : entList ) {
            auto& comp1 = st1.GetByEntityRef(ent);
            auto comp2 = st2.GetByEntity(ent);
            if (!comp2) continue;
            auto comp3 = st3.GetByEntity(ent);
            if (!comp3) continue;
            auto comp4 = st4.GetByEntity(ent);
            if (!comp4) continue;
            fn(comp1, *comp2, *comp3, *comp4);
        }
    }
}
