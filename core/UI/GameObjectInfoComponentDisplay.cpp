#include "GameObjectInfoComponentDisplay.h"
#include "../Core.h"
#include "../GameObject.h"

void TetraEngine::UI::GameObjectInfoComponentDisplay::Draw(ECS::TypeErasedHandle handle) {
    TETRA_USE_MAIN_ECS
    auto transformHandle = static_cast<ECS::Handle<GameObjectInfo>>(handle);
    GameObjectInfo* info = ecs.GetComponent<GameObjectInfo>(transformHandle);

    if (ImGui::CollapsingHeader("Info")) {

        ImGui::PushID("Info");
        ImGui::Text("Name: %s", info->name.c_str());
        if (ImGui::Checkbox("Enabled", &info->isEnabled)) {
        }
        ImGui::PopID();
    }
}
