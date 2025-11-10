#include "tetrapc.h"
#include "LightManager.h"

#include <string>
#include "Light.h"
#include "PointLight.h"
#include "Shader.h"
#include "../Core.h"
#include "../GameObject.h"
#include "../Transform.h"

using namespace TetraEngine;

void LightManager::dispatchPointLights(Shader* shader)
{
	shader->Use();

	shader->SetVec3("light.dir", mainLightDir);
	shader->SetVec3("light.ambient", mainLightAmbient);
	shader->SetVec3("light.diffuse", mainLightDiffuse);
	shader->SetVec3("light.specular", mainLightSpecular);

	shader->SetInt("pointLightsCount", pointLights.size());

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);

	glBufferData(GL_SHADER_STORAGE_BUFFER,
			 pointLights.size() * sizeof(PointLightInfo),
			 pointLights.data(),
			 GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

void LightManager::CollectLightData() {
	pointLights.clear();
	Core::GetMainECS().Foreach<PointLight, Transform, GameObjectInfo>(
	    [&](PointLight& pl, Transform& t, GameObjectInfo& info) {
		AddPointLight(pl, t, info);
	});
}


void LightManager::AddPointLight(PointLight& pl, Transform& transform, GameObjectInfo& gameObject) {
	if (!gameObject.isEnabled)
		return;
	pointLights.push_back(PointLightInfo{
		transform.GetPosition(), 0.0f,
		pl.ambient, 0.0,
		pl.diffuse, 0.0f,
		pl.specular,0.0f,
		pl.attenuation, 0.0f
	});
}

LightManager::LightManager() {
	glGenBuffers(1, &SSBO);
}

LightManager::~LightManager() {
	glDeleteBuffers(1, &SSBO);
}

