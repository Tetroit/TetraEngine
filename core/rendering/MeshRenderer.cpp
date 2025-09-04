#include "tetrapc.h"
#include "MeshRenderer.h"

#include "Shader.h"
#include "VertexData.h"
#include "Camera.h"
#include "Material.h"
#include "Skybox.h"
#include "Texture2D.h"
#include "Cubemap.h"

using namespace TetraEngine;

MeshRenderer* MeshRenderer::defaultRenderer;
MeshRenderer* MeshRenderer::skyboxRenderer;

MeshRenderer::MeshRenderer(std::shared_ptr<VertexData> vd, Shader* sh) : mesh(vd), shader(sh), camera(Camera::main), material(Material::defaultMaterial) {
    diffuseTexture = nullptr;
    specularTexture = nullptr;
    emissionTexture = nullptr;
    textureFlags = 0;
}

void MeshRenderer::Render(glm::mat4 transform) {

    textureFlags =
        (diffuseTexture != nullptr) +
        ((specularTexture != nullptr) << 1) +
        ((emissionTexture != nullptr) << 2) +
        ((Skybox::current != nullptr) << 3);

    if (mesh == nullptr) {

        mesh = VertexData::GetPrefab(0);
        shader = Shader::billboardShader;
    }
    glm::mat4 view = Camera::main->GetViewMatrix();

    if (Shader::currentShader != shader)
        shader->Use();

    if (material != nullptr) {
        shader->SetVec3("surface.ambient", material->ambient);
        shader->SetVec3("surface.diffuse", material->diffuse);
        shader->SetVec3("surface.specular", material->specular);
        shader->SetFloat("surface.shininess", material->shininess);
    }
    shader->SetInt("textureFlags", textureFlags);

    shader->SetMat4("projection", Camera::main->projectionView);
    shader->SetMat4("view", view);
    shader->SetMat4("transform", transform);

    if (diffuseTexture) 
    {
        shader->SetInt("textureD", 0);
        diffuseTexture->Bind(0);
    }
    if (specularTexture) 
    {
        shader->SetInt("textureS", 1);
        specularTexture->Bind(1);
    }
    if (emissionTexture) 
    {
        shader->SetInt("textureA", 2);
        emissionTexture->Bind(2);
    }

    if (Skybox::current != nullptr)
    {
        shader->SetInt("skyTexture", 3);
        Skybox::current->cubemap->Bind(3);
    }
    mesh->Draw();

    if (emissionTexture) specularTexture->Unbind(2);
    if (specularTexture) specularTexture->Unbind(1);
    if (diffuseTexture) diffuseTexture->Unbind(0);
}

void MeshRenderer::setTexture(Texture2D* tex, int texBit)
{
    if ((texBit & 1) != 0) diffuseTexture = tex;
    if ((texBit & 2) != 0) specularTexture = tex;
    if ((texBit & 4) != 0) emissionTexture = tex;
}
void MeshRenderer::setTexture(const std::string& path)
{
    diffuseTexture = new Texture2D();
    diffuseTexture->Load(path);
}

void MeshRenderer::InitialiseRenderer() {
    defaultRenderer = new MeshRenderer(VertexData::GetPrefab(VD_RECTANGLE), Shader::billboardShader);
    defaultRenderer->setTexture(assetPath + "/default.png");
    skyboxRenderer = new MeshRenderer(VertexData::GetPrefab(VD_RECTANGLE), Shader::skysphereShader);
    skyboxRenderer->setTexture(assetPath + "/skybox.jpg");
}