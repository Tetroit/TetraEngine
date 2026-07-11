#include "ResourceView.h"

namespace TetraEngine {
    void UI::ResourceView::Display() {
        if (ImGui::Button("Load")) {
            nfdchar_t *outPath = NULL;
            nfdresult_t result = NFD_OpenDialog(&outPath, NULL, 0, NULL);
            if (result == NFD_OKAY) {
                ImGui::Text("Success");
                std::filesystem::path path(outPath);
                std::filesystem::path extension = path.extension();
                std::filesystem::path relative = std::filesystem::relative(path);

                if (extension == ".png" ||
                    extension == ".jpg" ||
                    extension == ".jpeg") {
                    content->Load<Texture2D>(relative);
                }
                delete outPath;
            }
        }
        if (ImGui::BeginTabBar("Types")) {
            for (auto& type : content->GetResourceTypes()) {
                if (ImGui::BeginTabItem(type.name)) {
                    DisplayItems(type.id);
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }

    }

    void UI::ResourceView::DisplayItems(asset_type_id type) {
        if (type == TypeInfo<Texture2D>::id) {
            ImVec2 itemSize = ImVec2(120, 140);
            ImVec2 contentSize = ImGui::GetContentRegionAvail();
            int cols = std::max<int>(contentSize.x / itemSize.x, 1);
            int itemN = 0;
            if (ImGui::BeginTable("TextureGrid", cols, ImGuiTableFlags_SizingFixedFit))
            {
                int i = 0;

                content->Foreach<Texture2D>([&](Texture2D& tex)
                {
                    ImGui::TableNextColumn();
                    DisplayItem<Texture2D>(tex);
                    i++;
                });

                ImGui::EndTable();
            }
        }
        else if (type == TypeInfo<Material>::id) {
            if (ImGui::BeginTable("MaterialTable", 1, ImGuiTableFlags_SizingFixedFit)) {

                content->Foreach<Material>([&](Material& tex)
                {
                    ImGui::TableNextColumn();
                    DisplayItem<Material>(tex);
                });
                ImGui::EndTable();
            }
        }
    }

    template<>
    void UI::ResourceView::DisplayItem<Texture2D>(Texture2D &item) {
        ImGui::Image(item.ID(), ImVec2(100,100), ImVec2(0, 0), ImVec2(1, 1));
        ImGui::Text("%ix%i", item.width, item.height);
    }
    template<>
    void UI::ResourceView::DisplayItem<Material>(Material &item) {
        ImGui::Text("%s", item.name.c_str());
    }
} // TetraEngine