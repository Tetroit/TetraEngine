#include "tetrapc.h"
#include "Light.h"

#include <stdexcept>

#include "LightManager.h"
#include "MeshRenderer.h"
#include "LightRenderer.h"

using namespace TetraEngine;

Light::Light(LightType type, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
ambient(ambient), diffuse(diffuse), specular(specular), lightType(type) {
}
