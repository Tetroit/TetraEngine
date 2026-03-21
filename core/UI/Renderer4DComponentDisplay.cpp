#include "Renderer4DComponentDisplay.h"
#include "../ecs/ECS.h"
#include "../Core.h"
#include "../rendering/Material.h"
#include "../rendering/4d/Renderer4D.h"
#include "../rendering/Shader.h"
#include "../rendering/Texture2D.h"

namespace TetraEngine::UI{
    void Renderer4DComponentDisplay::DisplayTextureInfo(Texture2D *tex, std::string&& textureName) {
        if (tex) {

            ImGui::TextWrapped("%s texture: %s", textureName.c_str(), tex->GetPath().c_str());
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.2f, 1.0f), "%dx%d", tex->width, tex->height);
            ImGuiManager::DrawTexture2D(*tex, 256, 256);
        }
        else {
            ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%s texture is missing", textureName.c_str());
        }
    }

    void Renderer4DComponentDisplay::Draw(ECS::TypeErasedHandle handle) {
        TETRA_USE_MAIN_ECS
        auto transformHandle = static_cast<ECS::Handle<Renderer4D>>(handle);
        auto* renderer = ecs.GetComponent<Renderer4D>(transformHandle);

        if (ImGui::CollapsingHeader("Renderer")) {

            ImGui::PushID("Renderer4D");
            if (ImGui::TreeNode("4D")) {
                ImGui::Text("Is visible between %.3f and %.3f", renderer->GetMinW(), renderer->GetMaxW());
                ImGui::DragFloat("Slice W", renderer->GetSliceWPtr(), 0.01f);

                const char* labels[4] = { "X", "Y", "Z", "W" };
                if (ImGui::BeginTable("Euler 4D", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit))
                {

                    ImGui::TableSetupColumn("##row_label", ImGuiTableColumnFlags_WidthFixed, 30.0f);
                    for (int i = 0; i < 4; i++)
                    {
                        ImGui::TableSetupColumn("##col", ImGuiTableColumnFlags_WidthStretch);
                    }
                    // --- Header row ---
                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted(""); // top-left empty corner

                    for (int col = 0; col < 4; col++)
                    {
                        ImGui::TableSetColumnIndex(col + 1);
                        ImGui::TextUnformatted(labels[col]);
                    }

                    int valID = 0;
                    // --- Matrix rows ---
                    for (int row = 0; row < 4; row++)
                    {
                        ImGui::TableNextRow();

                        // Row label
                        ImGui::TableSetColumnIndex(0);
                        ImGui::TextUnformatted(labels[row]);

                        // Matrix values
                        for (int col = row+1; col < 4; col++)
                        {
                            if (col == row) continue;
                            ImGui::TableSetColumnIndex(col + 1);

                            ImGui::PushID(row * 4 + col);

                            ImGui::DragFloat("##v", renderer->GetRotationPtr(valID), 0.01f);
                            valID++;

                            ImGui::PopID();
                        }
                    }

                    ImGui::EndTable();
                }
                ImGui::DragFloat4("Position 4D", renderer->GetPositionPtr(), 0.01f);
                ImGui::TreePop();
            }
            // if (ImGui::TreeNode("Textures")) {
            //
            //     DisplayTextureInfo(renderer->textureEmission, "Emissive");
            //     DisplayTextureInfo(renderer->textureDiffuse, "Diffuse");
            //     DisplayTextureInfo(renderer->textureSpecular, "Specular");
            //
            //     ImGui::TreePop();
            // }
            //
            // if (ImGui::TreeNode("Material")) {
            //
            //     if (renderer->material) {
            //         ImGui::ColorEdit3("ambient", &renderer->material->ambient.x, ImGuiColorEditFlags_NoInputs);
            //
            //         ImGui::ColorEdit3("diffuse", &renderer->material->diffuse.x, ImGuiColorEditFlags_NoInputs);
            //
            //         // if (renderer->diffuseTexture) {
            //         //     ImGui::SameLine();
            //         //     ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.2f, 1), "Isn't being used because diffuse texture is active");
            //         // }
            //
            //         ImGui::ColorEdit3("specular", &renderer->material->specular.x, ImGuiColorEditFlags_NoInputs);
            //
            //         // if (renderer->specularTexture) {
            //         //     ImGui::SameLine();
            //         //     ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.2f, 1), "Isn't being used because specular texture is active");
            //         // }
            //     }
            //     else {
            //         ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "Error reading material");
            //     }
            //
            //     ImGui::TreePop();
            // }

            if (ImGui::TreeNode("Shader")) {
                if (renderer->shader) {

                    if (renderer->shader != prefs.shaderContext) {
                        prefs.shaderContext = renderer->shader;
                        prefs.vertexPath = prefs.shaderContext->GetVertexPath();
                        prefs.vertexCode = prefs.shaderContext->VertexAsString();
                        prefs.fragmentPath = prefs.shaderContext->GetFragmentPath();
                        prefs.fragmentCode = prefs.shaderContext->FragmentAsString();
                        if (renderer->shader->HasGeometry()) {
                            prefs.geometryCode = prefs.shaderContext->GetGeometryPath();
                            prefs.geometryCode = prefs.shaderContext->GeometryAsString();
                        }
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
                    if (renderer->shader->HasGeometry()) {
                        if (ImGui::TreeNode("Geometry")) {

                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 1.0f, 1.0f));
                            ImGui::TextWrapped("Loaded geometry shader at path\n %s", prefs.geometryPath.c_str());
                            ImGui::PopStyleColor();
                            ImGui::TextUnformatted(prefs.geometryCode.c_str());

                            ImGui::TreePop();
                        }
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
