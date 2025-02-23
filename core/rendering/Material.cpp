#include "tetrapc.h"
#include "Material.h"

#include <fstream>
#include <sstream>
#include <iostream>

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