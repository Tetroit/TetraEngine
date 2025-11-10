#pragma once
#include  "tetrapc.h"


namespace TetraEngine {
    namespace ECS {

        constexpr uint INVALID_ENTITY = std::numeric_limits<uint>::max();
        using entityID = uint;

        struct Entity {
            entityID ID = INVALID_ENTITY;
            uint gen = 0;

            Entity() = default;
            Entity(const Entity&) = default;
            Entity(entityID ID, uint gen) : ID(ID), gen(gen) {}
            [[nodiscard]]
            bool isValid() const;
            [[nodiscard]]
            bool operator == (const Entity& other) const;
        };
    }
}

namespace std {
    template <>
    struct hash<TetraEngine::ECS::Entity> {
        size_t operator()(const TetraEngine::ECS::Entity& e) const noexcept {
            return std::hash<uint32_t>{}(e.ID);
        }
    };
}