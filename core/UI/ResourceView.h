#pragma once
#include "imgui.h"
#include "nfd.h"
#include "../resources/SharedContentRegistry.h"

namespace TetraEngine {
    namespace UI {
        class ResourceView {
            SharedContentRegistry* content;
        public:
            explicit ResourceView(SharedContentRegistry* content) : content(content) {}

            void Display();
            void DisplayItems(asset_type_id type);
            template<typename T>
            void DisplayItem(T& item) {
                ImGui::Text(TypeInfo<T>::name);
            }
            template<>
            void DisplayItem(Texture2D& item);
            template<>
            void DisplayItem(Material& item);
        };
    }
}
