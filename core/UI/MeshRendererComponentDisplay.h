#pragma once
#include "ComponentDisplay.h"


namespace TetraEngine {
    class Shader;
}
namespace TetraEngine::UI {

    struct MeshRendererComponentDisplayPrefs {
        Shader* shaderContext = nullptr;
        std::string vertexPath = "";
        std::string fragmentPath = "";
        std::string vertexCode = "";
        std::string fragmentCode = "";
    };

    class MeshRendererComponentDisplay : public ComponentDisplay {
    private:
        MeshRendererComponentDisplayPrefs prefs;
        void DisplayTextureInfo(Texture2D* tex, std::string&& textureName);
    public:
        void Draw(ECS::TypeErasedHandle handle) override;
    };
}
