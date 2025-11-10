#pragma once
#include "imgui.h"
#include "../ecs/Entity.h"

#include "TransformComponentDisplay.h"
#include "GameObjectInfoComponentDisplay.h"
#include "RigidBodyComponentDisplay.h"
#include "MeshRendererComponentDisplay.h"
#include "PointLightComponentDisplay.h"

namespace TetraEngine {
    class Transform;
    class GameObjectInfo;
}
namespace TetraEngine::ECS {
    struct Entity;
    struct TypeErasedHandle;
}



namespace TetraEngine::UI {
    class Inspector {
        std::unordered_map<uint, std::unique_ptr<ComponentDisplay>> displayMethodRegistry;
        std::unordered_map<uint, ECS::TypeErasedHandle> handleLinks;
        ECS::Entity context;
    public:
        void Display(const ECS::Entity& entity, bool* isDrawn);
        void Display(const ECS::Entity& entity);
        template<class T, class U>
        void RegisterComponentDisplay();
    };
}
// TetraEngine::UI

#include <../ecs/ECS.h>

namespace TetraEngine::UI {

    template<class T, class U>
    void Inspector::RegisterComponentDisplay() {
        static_assert(std::is_base_of_v<ComponentDisplay, U>, "U must derive from ComponentDisplay");
        displayMethodRegistry[typeid(T).hash_code()] = std::make_unique<U>();
    }

}