#pragma once

#include <glm/glm.hpp>
#include <string>
#include <variant>
#include <vector>

#define TETRA_MATERIAL_TYPE_FALLBACK_GET(_it, _name, _fallback, _type)\
if (_it == properties.end()) {\
    std::cout << "Material property " << _name << " not found" << std::endl;\
    return _fallback;\
}\
if (it->second.first != _type) {\
    std::cout << "[WARN] Material property " << _name << " is not the same type (" << #_type << ")" << std::endl;\
}

#define TETRA_MATERIAL_TYPE_FALLBACK_SET(_it, _name, _type)\
if (it->second.first != _type) {\
    std::cout << "[WARN] Material property " << _name << " is not the same type (" << #_type << ")" << std::endl;\
    }

namespace TetraEngine {

    using MaterialProperty = std::variant<bool, int, float, glm::vec2, glm::vec3, glm::vec4>;
    enum EMaterialPropertyType {
        BOOL,
        INT,
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        COLOR,
        TEX2D, //soon
    };
    class Material
    {
    private:
        static int lastId;

        int id;

    public:
        static Material* defaultMaterial;
        static std::vector<Material> collection;
        static void Initialize();
        static void ParseMTL(std::string path);
        static Material* GetBYName(std::string name);

        std::unordered_map<std::string, std::pair<EMaterialPropertyType, MaterialProperty>> properties;
        glm::vec3 ambient = glm::vec3(1.f, 1.f, 1.f);
        glm::vec3 diffuse = glm::vec3(1.f, 1.f, 1.f);
        glm::vec3 specular = glm::vec3(1.f, 1.f, 1.f);
        float shininess = 16;
        std::string name;
        Material(std::string name) : name(name) { id = lastId++; }
        Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::string name = "material") : ambient(ambient), diffuse(diffuse), specular(specular), name(name) { id = lastId++; }
        bool HasProperty(std::string name) {return properties.find(name) != properties.end();}
        bool GetBool(std::string name) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, false, BOOL)
            return std::get<bool>(it->second.second);
        }
        int GetInt(std::string name) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, 0, INT)
            return std::get<int>(it->second.second);
        }
        float GetFloat(std::string name) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, 0.0f, FLOAT)
            return std::get<float>(it->second.second);
        }
        glm::vec2 GetVec2(std::string name) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, glm::vec2(), VEC2)
            return std::get<glm::vec2>(it->second.second);
        }
        glm::vec3 GetVec3(std::string name) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, glm::vec3(), VEC3)
            return std::get<glm::vec3>(it->second.second);
        }
        glm::vec4 GetVec4(std::string name) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, glm::vec4(), VEC4)
            return std::get<glm::vec4>(it->second.second);
        }
        glm::vec4 GetColor(std::string name) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, glm::vec4(), COLOR)
            return std::get<glm::vec4>(it->second.second);
        }

        void SetBool(std::string name, bool value) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, BOOL)
            it->second.second = value;
        }
        void SetInt(std::string name, int value) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, INT)
            it->second.second = value;
        }
        void SetFloat(std::string name, float value) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, FLOAT)
            it->second.second = value;
        }
        void SetVec2(std::string name, glm::vec2 value) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, VEC2)
            it->second.second = value;
        }
        void SetVec3(std::string name, glm::vec3 value) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, VEC3)
            it->second.second = value;
        }
        void SetVec4(std::string name, glm::vec4 value) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, VEC4)
            it->second.second = value;
        }
        void SetColor(std::string name, glm::vec4 value) {
            auto it = properties.find(name);
            TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, COLOR)
            it->second.second = value;
        }

    };
}


