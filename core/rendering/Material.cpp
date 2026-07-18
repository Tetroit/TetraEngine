#include "tetrapc.h"
#include "Material.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "../Core.h"
#include "../utils/Utils.h"

using namespace TetraEngine;

Material* Material::defaultMaterial = nullptr;
std::vector<Material> Material::collection;
int Material::lastId = 0;

void Material::Initialize() {
	collection.push_back(Material("default"));
	defaultMaterial = &collection[0];
}
Material* Material::GetBYName(std::string name) {
    for (Material mat : collection)
    {
        if (mat.name == name)
            return &mat;
    }
    return nullptr;
}

bool Material::GetBool(std::string name) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, false, BOOL)
    return std::get<bool>(it->second.second);
}

int Material::GetInt(std::string name) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, 0, INT)
    return std::get<int>(it->second.second);
}

float Material::GetFloat(std::string name) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, 0.0f, FLOAT)
    return std::get<float>(it->second.second);
}

glm::vec2 Material::GetVec2(std::string name) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, glm::vec2(), VEC2)
    return std::get<glm::vec2>(it->second.second);
}

glm::vec3 Material::GetVec3(std::string name) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, glm::vec3(), VEC3)
    return std::get<glm::vec3>(it->second.second);
}

glm::vec4 Material::GetVec4(std::string name) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, glm::vec4(), VEC4)
    return std::get<glm::vec4>(it->second.second);
}

glm::vec4 Material::GetColor(std::string name) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_GET(it, name, glm::vec4(), COLOR)
    return std::get<glm::vec4>(it->second.second);
}

void Material::SetBool(std::string name, bool value) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, BOOL)
    it->second.second = value;
}

void Material::SetInt(std::string name, int value) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, INT)
    it->second.second = value;
}

void Material::SetFloat(std::string name, float value) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, FLOAT)
    it->second.second = value;
}

void Material::SetVec2(std::string name, glm::vec2 value) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, VEC2)
    it->second.second = value;
}

void Material::SetVec3(std::string name, glm::vec3 value) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, VEC3)
    it->second.second = value;
}

void Material::SetVec4(std::string name, glm::vec4 value) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, VEC4)
    it->second.second = value;
}

void Material::SetColor(std::string name, glm::vec4 value) {
    auto it = properties.find(name);
    TETRA_MATERIAL_TYPE_FALLBACK_SET(it, name, COLOR)
    it->second.second = value;
}

void Material::FromJSON(nlohmann::json json) {
    for (auto propertyBlock : json["properties"]) {
        std::string pName = propertyBlock["name"];
        std::string pType = propertyBlock["type"];
        if (pType == "bool") {
            bool val = propertyBlock["value"].get<bool>();
            properties.emplace(name, std::make_pair(BOOL, val));
        }
        else if (pType == "int") {
            int val = propertyBlock["value"].get<int>();
            properties.emplace(name, std::make_pair(INT, val));
        }
        else if (pType == "float") {
            float val = propertyBlock["value"].get<float>();
            properties.emplace(name, std::make_pair(FLOAT, val));
        }
        else if (pType == "vec2") {
            glm::vec2 val = {
                propertyBlock["value"]["x"].get<float>(),
                propertyBlock["value"]["y"].get<float>()
            };
            properties.emplace(name, std::make_pair(VEC2, val));
        }
        else if (pType == "vec3") {
            glm::vec3 val = {
                propertyBlock["value"]["x"].get<float>(),
                propertyBlock["value"]["y"].get<float>(),
                propertyBlock["value"]["z"].get<float>()
            };
            properties.emplace(name, std::make_pair(VEC3, val));
        }
        else if (pType == "vec4" || pType == "color") {
            glm::vec4 val = {
                propertyBlock["value"]["x"].get<float>(),
                propertyBlock["value"]["y"].get<float>(),
                propertyBlock["value"]["z"].get<float>(),
                propertyBlock["value"]["w"].get<float>()
            };
            properties.emplace(name, std::make_pair(pType == "vec4" ? VEC4 : COLOR, val));
        }
        else if (pType == "texture2D") {
        }
    }
}

void Material::ParseMTL(std::string path) {

    std::string line;
	std::ifstream stream = std::ifstream(path);
	if (!stream.good())
		std::cout << "file couldnt open";
    Material* context = nullptr;

    while (!stream.eof())
    {
        std::getline(stream, line);
        std::vector<std::string> words;
        Utils::Words(line, words, ' ');
        if (words.size() > 0) {
            if (words[0] == "newmtl") {
                collection.push_back(Material(words[1]));
                context = &collection[collection.size() - 1];
            }
            if (words[0] == "Ka") {
                context->ambient = glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
            }
            if (words[0] == "Kd") {
                context->diffuse = glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
            }
            if (words[0] == "Ks") {
                context->specular = glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
            }
            if (words[0] == "Ns") {
                context->shininess = std::stof(words[1]);
            }
        }
    }
}
