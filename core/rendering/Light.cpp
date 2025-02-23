#include "tetrapc.h"
#include "Light.h"

#include <stdexcept>

#include "MeshRenderer.h"
#include "LightRenderer.h"

using namespace TetraEngine;

void Light::Render() {
    LightRenderer* lr = static_cast<LightRenderer*>(renderer);
    if (lr)
        lr->light = LightInfo(ambient, diffuse, specular);
    else
        throw std::invalid_argument("renderer is not set to LightRenderer");
    GameObject::Render();
}
Light::Light(glm::vec3 pos, const std::string name = "object", LightRenderer* meshRenderer) : GameObject(pos, name, meshRenderer) {
    ambient = glm::vec3(0);
    diffuse = glm::vec3(0);
    specular = glm::vec3(0);
}

Light* Light::Create(glm::vec3 pos, const std::string name = "object", LightRenderer* meshRenderer) {
    Light* ptr(new Light(pos, name, meshRenderer));
    return ptr;
}