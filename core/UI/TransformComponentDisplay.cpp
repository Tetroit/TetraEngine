
#include "TransformComponentDisplay.h"

#include "Inspector.h"
#include "../ecs/ECS.h"
#include "../Core.h"

namespace TetraEngine::UI {

    void TransformComponentDisplay::Draw(ECS::TypeErasedHandle handle) {
        TETRA_USE_MAIN_ECS
        auto transformHandle = static_cast<ECS::Handle<Transform>>(handle);
        Transform* tr = ecs.GetComponent<Transform>(transformHandle);

        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::PushID("Transform");
            auto pos = glm::vec3();
            auto rot = glm::vec3();
            auto scale = glm::vec3();
            ImGui::Checkbox("Show local", &displayPrefs.showLocal);

            if (displayPrefs.showLocal) {

                pos = tr->GetLocalPosition();
                rot = glm::eulerAngles(tr->GetLocalRotation());
                scale = tr->GetLocalScale();
            }
            else {
                pos = tr->GetPosition();
                rot = glm::eulerAngles(tr->GetRotation());
                scale = tr->GetScale();
            }
            ImGui::DragFloat3("Position", &pos.x, 0.01f, -1000.0f, 1000.0f);
            ImGui::DragFloat3("Rotation", &rot.x, 0.01f, -1.0f, 1.0f);
            ImGui::DragFloat3("Scale", &scale.x, 0.01f, -1000.0f, 1000.0f);

            if (displayPrefs.showLocal) {
                tr->SetLocalPosition(pos);
            }
            else {
                tr->SetPosition(pos);
            }
            ImGui::PopID();
        }
    }

}
