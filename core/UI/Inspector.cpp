#include "Inspector.h"

#include "../Core.h"
#include "../ecs/ECS.h"

namespace TetraEngine::UI {

    void Inspector::Display(const ECS::Entity& entity, bool* isDrawn) {

        if (ImGui::Begin("Inspector", isDrawn)) {
            Display(entity);
        }
        ImGui::End();
    }

    void Inspector::Display(const ECS::Entity& entity) {
        TETRA_USE_MAIN_ECS

        if (entity != context) {

            handleLinks.clear();
            context = entity;
        }

        if (!context.isValid()) {
            ImGui::Text("No object selected");
            return;
        }

        std::string entityDisplay = "Entity" + std::to_string(entity.ID);
        ImGui::Text("Entity: %d (%d)", entity.ID, entity.gen);

        for (auto& [id, handle] : ecs.GetAllEntityComponents(entity) ) {
            handleLinks.insert_or_assign(id, handle);
        }

        for (auto& [id, handle] : handleLinks) {
            if (!displayMethodRegistry.contains(id)) {
                ImGui::Text("No inspector method assigned for %d", id);
                return;
            }
            auto* method = displayMethodRegistry.at(id).get();
            if (method == nullptr) {
                return;
            }
            method->Draw(handle);
        }
    }
} // TetraEngine::UI