#include "tetrapc.h"
#include "LightManager.h"

#include <string>
#include "Light.h"
#include "PointLight.h"
#include "Shader.h"

using namespace TetraEngine;

void LightManager::fetchPointLights(Shader* shader)
{
	shader->SetVec3("light.dir", mainLightDir);
	shader->SetVec3("light.ambient", mainLightAmbient);
	shader->SetVec3("light.diffuse", mainLightDiffuse);
	shader->SetVec3("light.specular", mainLightSpecular);

	shader->SetInt("pointlightsCount", pointLights.size());
	for (int i = 0; i < pointLights.size(); i++)
	{
		shader->SetVec3(std::string("pointLights[" + std::to_string(i) + "].pos"), pointLights[i]->GetPos());
		shader->SetVec3(std::string("pointLights[" + std::to_string(i) + "].ambient"), pointLights[i]->ambient);
		shader->SetVec3(std::string("pointLights[" + std::to_string(i) + "].diffuse"), pointLights[i]->diffuse);
		shader->SetVec3(std::string("pointLights[" + std::to_string(i) + "].specular"), pointLights[i]->specular);
		shader->SetVec3(std::string("pointLights[" + std::to_string(i) + "].attenuation"), pointLights[i]->attenuation);
	}
}

int LightManager::AddPointLight(PointLight* pl)
{
	pointLights.push_back(pl);
	return pointLights.size() - 1;
}

void LightManager::RemovePointLight(int id)
{
	for (int i = id; i < pointLights.size(); i++)
	{
		pointLights[i]->lightId--;
	}
	pointLights.erase(pointLights.begin() + id);
}
