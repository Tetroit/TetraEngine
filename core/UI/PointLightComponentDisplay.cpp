
#include "PointLightComponentDisplay.h"
#include "../Core.h"
#include "../rendering/PointLight.h"

namespace TetraEngine {
    namespace UI {
        float PointLightComponentDisplay::AttenuationGraph(void* data, int i) {
            auto att = static_cast<float*>(data);
            float step = static_cast<float>(i) * 0.1f;
            return 1 / (att[0] + att[1] * step + att[2] * step * step);
        }

        void PointLightComponentDisplay::Draw(ECS::TypeErasedHandle handle) {

            TETRA_USE_MAIN_ECS
            auto transformHandle = static_cast<ECS::Handle<PointLight>>(handle);
            PointLight* light = ecs.GetComponent<PointLight>(transformHandle);

            if (ImGui::CollapsingHeader("PointLight")) {

                ImGui::PushID("PointLight");
                ImGui::ColorEdit3("Ambient", &light->ambient.x, ImGuiColorEditFlags_None);
                ImGui::ColorEdit3("Diffuse", &light->diffuse.x, ImGuiColorEditFlags_None);
                ImGui::ColorEdit3("Specular", &light->specular.x, ImGuiColorEditFlags_None);
                ImGui::DragFloat3("Attenuation", &light->attenuation.x, 0.01f, 0.0f, 100.0f);

                ImGui::PlotLines("AttGraph", AttenuationGraph, &light->attenuation.x, 100, 0, NULL, 0, 1.0f, ImVec2(0, 80));

                ImGui::PopID();
            }
        }
    } // UI
} // TetraEngine