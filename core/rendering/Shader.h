#pragma once

#include <string>
#include <glm/glm.hpp>

namespace TetraEngine {
    class Shader
    {
        std::string vertexPath;
        std::string fragmentPath;
    public:
        static Shader* billboardShader;
        static Shader* skysphereShader;
        static Shader* skyboxShader;
        static Shader* textShader;
        static Shader* currentShader;

        uint ID;
        bool isUpdatedThisFrame = false;

        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        void Use();
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetMat4(const std::string& name, glm::mat4& value) const;
        void SetVec3(const std::string& name, const glm::vec3& value) const;
        void SetVec4(const std::string& name, glm::vec4& value) const;
        void SetVec3(const std::string& name, float x, float y, float z) const;
        void SetVec4(const std::string& name, float x, float y, float z, float w) const;
        uint GetID() const;

        std::string GetVertexPath() const;
        std::string GetFragmentPath() const;
        std::string VertexAsString() const;
        std::string FragmentAsString() const;

        static void InitialiseShaders();
    };
}