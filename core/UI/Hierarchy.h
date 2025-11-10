#pragma once

#include "tetrapc.h"
#include <imgui.h>

#include "../Transform.h"
#include "../ecs/ECS.h"

namespace TetraEngine {

    class Transform;
    class GameObjectInfo;

    namespace UI {

        struct GameObjectTreeEntry {

            ECS::Entity selfID;
            const char *name;
            ECS::Entity parentID;
            std::vector<ECS::Entity> children;
            int idInParent;
            bool expanded;

            GameObjectTreeEntry(const Transform& tr, const GameObjectInfo& info);
            ~GameObjectTreeEntry();
        };

        class Hierarchy {

            std::unordered_map<ECS::Entity, GameObjectTreeEntry> nodes;
            std::vector<ECS::Entity> roots;
            ECS::Entity selected = ECS::Entity();
            Action<ECS::Entity> onSelectAction;

        public:

            int HierarchyIDStack;

            void Draw(bool* isDrawn);
            void Draw(const GameObjectTreeEntry& node);
            void BindEvents();

            void OnDestroy (TETRA_COMPONENT_EVENT_LISTENER_PARAMS(Transform));

            [[nodiscard]]
            ECS::Entity GetSelected();
            Action<ECS::Entity>& OnSelected();

            void ParentNodes(ECS::Entity parent, ECS::Entity child);
            void ParseInfo();
            GameObjectTreeEntry* BuildTreeFrom(const Transform& tr, const GameObjectInfo& info);
        };
    }
}


