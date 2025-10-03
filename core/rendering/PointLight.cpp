#include "tetrapc.h"
#include "PointLight.h"

#include <iostream>

#include "Light.h"
#include "Scene.h"

using namespace TetraEngine;


PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation) :
Light(Light::LightType::POINT, ambient, diffuse, specular), attenuation(attenuation) {

}
