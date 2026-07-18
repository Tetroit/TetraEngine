#pragma once

#include <glm/glm.hpp>
#include <string>
#include <variant>
#include <vector>

#include "nlohmann/json.hpp"

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

    using MaterialProperty = std::variant<bool, int, float, glm::vec2, glm::vec3, glm::vec4, GLuint>;
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
    struct MaterialPropertySerialized {
        std::string type;
        std::string name;
        MaterialProperty value;
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
        static void ParseMTL(std::string path); //wip
        static Material LoadFromJSON(std::string json);
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
        bool GetBool(std::string name);
        int GetInt(std::string name);
        float GetFloat(std::string name);
        glm::vec2 GetVec2(std::string name);
        glm::vec3 GetVec3(std::string name);
        glm::vec4 GetVec4(std::string name);
        glm::vec4 GetColor(std::string name);

        void SetBool(std::string name, bool value);
        void SetInt(std::string name, int value);
        void SetFloat(std::string name, float value);
        void SetVec2(std::string name, glm::vec2 value);
        void SetVec3(std::string name, glm::vec3 value);
        void SetVec4(std::string name, glm::vec4 value);
        void SetColor(std::string name, glm::vec4 value);

        void FromJSON(nlohmann::json json);
        std::string ToJSON(std::string json);
    };
}


