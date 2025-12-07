//
// Created by pchyo on 15/10/2025.
//

#include "MeshRendererComponentDisplay.h"
#include "../ecs/ECS.h"
#include "../Core.h"
#include "../rendering/Material.h"
#include "../rendering/MeshRenderer.h"
#include "../rendering/Shader.h"
#include "../rendering/Texture2D.h"

namespace TetraEngine::UI{
    void MeshRendererComponentDisplay::DisplayTextureInfo(Texture2D *tex, std::string&& textureName) {
        if (tex) {

            ImGui::TextWrapped("%s texture: %s", textureName.c_str(), tex->GetPath().c_str());
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.2f, 1.0f), "%dx%d", tex->width, tex->height);
            ImGuiManager::DrawTexture2D(*tex, 256, 256);
        }
        else {
            ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%s texture is missing", textureName.c_str());
        }
    }

    void MeshRendererComponentDisplay::Draw(ECS::TypeErasedHandle handle) {
        TETRA_USE_MAIN_ECS
        auto transformHandle = static_cast<ECS::Handle<MeshRenderer>>(handle);
        MeshRenderer* renderer = ecs.GetComponent<MeshRenderer>(transformHandle);

        if (ImGui::CollapsingHeader("Renderer")) {

            ImGui::PushID("RigidBody");
            if (ImGui::TreeNode("Textures")) {

                DisplayTextureInfo(renderer->textureEmission, "Emissive");
                DisplayTextureInfo(renderer->textureDiffuse, "Diffuse");
                DisplayTextureInfo(renderer->textureSpecular, "Specular");

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Material")) {

                if (renderer->material) {
                    ImGui::ColorEdit3("ambient", &renderer->material->ambient.x, ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit3("diffuse", &renderer->material->diffuse.x, ImGuiColorEditFlags_NoInputs);

                    // if (renderer->diffuseTexture) {
                    //     ImGui::SameLine();
                    //     ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.2f, 1), "Isn't being used because diffuse texture is active");
                    // }

                    ImGui::ColorEdit3("specular", &renderer->material->specular.x, ImGuiColorEditFlags_NoInputs);

                    // if (renderer->specularTexture) {
                    //     ImGui::SameLine();
                    //     ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.2f, 1), "Isn't being used because specular texture is active");
                    // }
                }
                else {
                    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "Error reading material");
                }

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Shader")) {
                if (renderer->shader) {

                    if (renderer->shader != prefs.shaderContext) {
                        prefs.shaderContext = renderer->shader;
                        prefs.vertexPath = prefs.shaderContext->GetVertexPath();
                        prefs.vertexCode = prefs.shaderContext->VertexAsString();
                        prefs.fragmentPath = prefs.shaderContext->GetFragmentPath();
                        prefs.fragmentCode = prefs.shaderContext->FragmentAsString();
                    }

                    if (ImGui::TreeNode("Vertex")) {

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 1.0f, 1.0f));
                        ImGui::TextWrapped("Loaded vertex shader at path\n %s", prefs.vertexPath.c_str());
                        ImGui::PopStyleColor();
                        ImGui::TextUnformatted(prefs.vertexCode.c_str());

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Fragment")) {

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 1.0f, 1.0f));
                        ImGui::TextWrapped("Loaded fragment shader at path\n %s", prefs.fragmentPath.c_str());
                        ImGui::PopStyleColor();
                        ImGui::TextUnformatted(prefs.fragmentCode.c_str());

                        ImGui::TreePop();
                    }
                }
                else {
                    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "Error reading shader");
                }

                ImGui::TreePop();
            }

            ImGui::PopID();
        }
    }
}
