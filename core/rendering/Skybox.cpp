#include "tetrapc.h"
#include "Skybox.h"

#include "VertexData.h"
#include "Shader.h"
#include "Camera.h"
#include "Cubemap.h"
#include "Texture2D.h"

using namespace TetraEngine;

Skybox* Skybox::current = nullptr;

Skybox::Skybox(Mode mode, const std::string& path) : mode(mode)
{
	if (mode == SPHERE) {
		cubemap = nullptr;
		texture = new Texture2D();
		texture->Load(path);
		shader = Shader::skysphereShader;
		mesh = VertexData::GetPrefab(VD_RECTANGLE);
	}

	if (mode == BOX) {
		texture = nullptr;
		cubemap = new Cubemap();
		cubemap->Load(path);
		shader = Shader::skyboxShader;
		mesh = VertexData::GetPrefab(VD_CUBE);
	}
}

Skybox::~Skybox()
{
	if (mode == SPHERE)
		delete texture;
	if (mode == BOX)
		delete cubemap;
}

void Skybox::Render()
{
	glDepthMask(GL_FALSE);
	shader->Use();

	if (mode == SPHERE)
	{
		glm::mat4 inverseProj = glm::inverse(Camera::main->projectionView);
		glm::mat4 inverseView = glm::inverse(Camera::main->GetViewMatrix());
		shader->SetMat4("inverseProj", inverseProj);
		shader->SetMat4("inverseView", inverseView);
		shader->SetVec3("viewPos", Camera::main->Position);

		shader->SetInt("tex", 0);
		texture->Bind(0);
		mesh->Draw();
		texture->Unbind(0);
	}
	if (mode == BOX)
	{
		glm::mat4 proj = Camera::main->projectionView;
		glm::mat4 view = glm::mat4(glm::mat3(Camera::main->GetViewMatrix()));
		shader->SetMat4("projection", proj);
		shader->SetMat4("view", view);

		shader->SetInt("tex", 0);
		cubemap->Bind(0);
		mesh->Draw();
		cubemap->Unbind(0);
	}
	glDepthMask(GL_TRUE);
}
