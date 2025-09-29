#pragma once
#include "tetrapc.h"
#include "Entity.h"
#include "Handle.h"

namespace TetraEngine {
    namespace ECS {

        class IStorage {
            public:
                virtual ~IStorage() = default;
                virtual void RemoveFromEntity(const entityID& entity) = 0;
                virtual bool TryRemoveFromEntity(const entityID& entity) = 0;
                virtual bool HasEntity(entityID entity) const = 0;
                virtual int Count() const = 0;
        };

        template<class T>
        class Storage : public IStorage {

            std::vector<T> data;
            std::vector<uint> gen;
            std::vector<bool> occupied;
            std::queue<uint> freeSlots;

            std::unordered_map<entityID, uint> entityToComponent;
            std::vector<entityID> entities;

        public:
            template<typename ...Args>
            Handle<T> Create(entityID entity, Args&&... args);
            Handle<T> Add(entityID entity, T comp);

            std::vector<T>& GetData () const;
            bool IsOccupied(uint id) const;
            const std::vector<entityID>& GetEntities() const;

            void ValidateHandle(Handle<T>& handle) const;
            Handle<T> GetHandle(entityID entity) const;

            T* Get(Handle<T>& handle);
            T* GetByEntity(entityID entity);
            T& GetByEntityRef(entityID entity);
            bool TryGetByEntityRef(entityID entity, T& out);

            const T* GetConst(Handle<T>& handle) const;
            const T* GetConstByEntity(entityID entity) const;
            const T& GetConstByEntityRef(entityID entity) const;

            void RemoveAt(uint slot);
            bool TryRemoveAt(uint slot);
            void RemoveFromEntity(const entityID& entity) override;
            bool TryRemoveFromEntity(const entityID& entity) override;
            void Remove(Handle<T>& handle);
            bool TryRemove(Handle<T>& handle);

            [[nodiscard]]
            bool HasEntity(entityID entity) const override;
            int Count() const override;
            template<typename FN>
            void Foreach(FN&& fn);
        };


        template<class T>
        std::vector<T> & Storage<T>::GetData() const {
            return data;
        }

        template<class T>
        bool Storage<T>::IsOccupied(uint id) const {
            return occupied[id];
        }

        template<class T>
        const std::vector<entityID>& Storage<T>::GetEntities() const {
            return entities;
        }

        template<class T>
        template<typename ... Args>
        Handle<T> Storage<T>::Create(entityID entity, Args &&...args) {
            return Add(entity, T(std::forward<Args>(args)...));
        }

        template<class T>
        Handle<T> Storage<T>::Add(entityID entity, T comp) {

            uint slot;
            if (freeSlots.empty()) {
                slot = data.size();
                data.push_back(std::move(comp));
                gen.push_back(0);
                occupied.push_back(true);
                entities.push_back(entity);
            }
            else {
                slot = freeSlots.front();
                freeSlots.pop();
                occupied[slot] = true;
                data[slot] = std::move(comp);
                entities[slot] = entity;
            }
            entityToComponent[entity] = slot;
            return Handle<T>(slot, gen[slot]);
        }

        template<class T>
        void Storage<T>::ValidateHandle(Handle<T> &handle) const {
            if (!handle.Valid())
                return;
            if (!occupied[handle.slot]) {
                handle.Invalidate(); return;
            }
            if (handle.compGeneration != gen[handle.slot]) {
                handle.Invalidate(); return;
            }
        }

        template<class T>
        Handle<T> Storage<T>::GetHandle(entityID entity) const {
            auto loc = std::ranges::find(entities, entity);
            if (loc == entities.end()) {
                LOG_ERR("Entity doesn't have the component");
                return Handle<T>::CreateInvalid();
            }
            auto slot = entityToComponent.at(*loc);
            return Handle<T>(slot, gen[slot]);
        }

        template<class T>
        T * Storage<T>::Get(Handle<T> &handle) {
            ValidateHandle(handle);
            if (!handle.Valid()) return nullptr;
            return &data[handle.slot];
        }

        template<class T>
        T * Storage<T>::GetByEntity(entityID entity) {
            auto loc = std::ranges::find(entities, entity);
            if (loc == entities.end()) {
                return nullptr;
            }
            return &data[entityToComponent[*loc]];
        }

        template<class T>
        T& Storage<T>::GetByEntityRef(entityID entity) {
            return data[entityToComponent.at(entity)];
        }

        template<class T>
        bool Storage<T>::TryGetByEntityRef(entityID entity, T &out) {
            auto loc = std::ranges::find(entities, entity);
            if (loc == entities.end()) return false;
            out = data[entityToComponent[(*loc)]];
            return true;
        }

        template<class T>
        const T * Storage<T>::GetConst(Handle<T> &handle) const {
            ValidateHandle(handle);
            if (!handle.Valid()) return nullptr;
            return &data[handle.slot];
        }

        template<class T>
        const T * Storage<T>::GetConstByEntity(entityID entity) const {
            auto handle = GetHandle(entity);
            ValidateHandle(handle);
            if (!handle.Valid()) return nullptr;
            return &data[handle.slot];
        }

        template<class T>
        const T & Storage<T>::GetConstByEntityRef(entityID entity) const {
            return data[entityToComponent.at(entity)];
        }

        template<class T>
        void Storage<T>::RemoveAt(uint slot) {
            if (data.size() <= slot) {
                LOG_ERR("slot out of range");
                return;
            }
            occupied[slot] = false;
            freeSlots.push(slot);
            gen[slot]++;
            auto entity = entities[slot];
            entityToComponent.erase(entity);
            entities[slot] = INVALID_ENTITY;
        }

        template<class T>
        bool Storage<T>::TryRemoveAt(uint slot) {
            if (data.size() <= slot) {
                return false;
            }
            occupied[slot] = false;
            freeSlots.push(slot);
            gen[slot]++;
            auto entity = entities[slot];
            entityToComponent.erase(entity);
            entities[slot] = INVALID_ENTITY;
            return true;
        }

        template<class T>
        void Storage<T>::RemoveFromEntity(const entityID &entity) {
            if (!entityToComponent.contains(entity))
                LOG_ERR("Entity not found");
            RemoveAt(entityToComponent[entity]);
        }

        template<class T>
        bool Storage<T>::TryRemoveFromEntity(const entityID &entity) {
            if (!entityToComponent.contains(entity))
                return false;
            RemoveAt(entityToComponent[entity]);
            return true;
        }

        template<class T>
        void Storage<T>::Remove(Handle<T> &handle) {
            ValidateHandle(handle);
            if (handle.Valid())
                RemoveAt(handle.slot);
            else
                LOG_ERR("handle invalid, couldn't remove the component");
        }

        template<class T>
        bool Storage<T>::TryRemove(Handle<T> &handle) {
            ValidateHandle(handle);
            if (handle.Valid()) {
                RemoveAt(handle.slot);
                return true;
            }
            return false;
        }

        template<class T>
        bool Storage<T>::HasEntity(entityID entity) const {
            if (std::ranges::find(entities, entity) != entities.end())
                return true;
            return false;
        }

        template<class T>
        int Storage<T>::Count() const {
            return entities.size();
        }

        template<class T>
        template<typename FN>
        void Storage<T>::Foreach(FN &&fn) {
            for (int i=0; i<data.size(); i++) {
                if (!occupied[i]) continue;
                fn(data[i]);
            }
        }
    } // ECS
} // TetraEngine
