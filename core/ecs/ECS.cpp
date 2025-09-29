//
// Created by pchyo on 24/09/2025.
//

#include "ECS.h"
#include "tetrapc.h"


namespace TetraEngine::ECS {


    Entity ECS::CreateEntity() {
        if (freeIDs.empty()) {
            entityGenerations.push_back(0);
            return {(entityID)entityGenerations.size() - 1, entityGenerations.back() };
        }
        auto id = freeIDs.at(0);
        freeIDs.erase(freeIDs.begin());
        return {id, entityGenerations.at(id)};
    }

    bool ECS::ValidateEntity(Entity &entity) const {
        if (!entity.isValid()) {
            LOG_ERR("Invalid entity");
            return false;
        }
        if (entityGenerations[entity.ID] != entity.gen ) {
            LOG_ERR("Invalid entity");
            entity.ID = INVALID_ENTITY; return false;
        }
        return true;
    }
    bool ECS::ValidateEntitySilent(Entity &entity) {
        if (!entity.isValid()) {
            return false;
        }
        if (entityGenerations[entity.ID] != entity.gen ) {
            entity.ID = INVALID_ENTITY; return false;
        }
        return true;
    }
    void ECS::RemoveEntity(Entity &entity) {
        if (!ValidateEntity(entity)) {
            return;
        }
        entityGenerations.at(entity.ID)++;
        freeIDs.push_back(entity.ID);
        entity.ID = INVALID_ENTITY;

        for (auto &storage: storages | std::views::values) {
            storage->TryRemoveFromEntity(entity.ID);
        }
    }

    bool ECS::TryRemoveEntity(Entity &entity) {
        if (!ValidateEntity(entity)) {
            return false;
        }
        entityGenerations.at(entity.ID)++;
        freeIDs.push_back(entity.ID);
        entity.ID = INVALID_ENTITY;

        for (auto &storage: storages | std::views::values) {
            storage->RemoveFromEntity(entity.ID);
        }
    }
}
