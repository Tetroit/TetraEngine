#pragma once
#include "ComponentDisplay.h"

namespace TetraEngine {
    class Texture2D;
    class Shader;
}
namespace TetraEngine::UI {

    struct Renderer4DComponentDisplayPrefs {
        Shader* shaderContext = nullptr;
        std::string vertexPath = "";
        std::string fragmentPath = "";
        std::string geometryPath = "";
        std::string vertexCode = "";
        std::string fragmentCode = "";
        std::string geometryCode = "";
    };

    class Renderer4DComponentDisplay : public ComponentDisplay {
    private:
        Renderer4DComponentDisplayPrefs prefs;
        void DisplayTextureInfo(Texture2D* tex, std::string&& textureName);
    public:
        void Draw(ECS::TypeErasedHandle handle) override;
    };
}
