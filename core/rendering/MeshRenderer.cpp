#include "tetrapc.h"
#include "MeshRenderer.h"

#include "Shader.h"
#include "VertexData.h"
#include "ViewportCamera.h"
#include "Material.h"
#include "Skybox.h"
#include "Texture2D.h"
#include "Cubemap.h"

using namespace TetraEngine;

MeshRenderer* MeshRenderer::defaultRenderer;
MeshRenderer* MeshRenderer::skyboxRenderer;

MeshRenderer::MeshRenderer(std::shared_ptr<VertexData> vd, Shader* sh) : mesh(vd), shader(sh), camera(ViewProvider::GetCurrent()), material(Material::defaultMaterial) {
    textureDiffuse = nullptr;
    textureSpecular = nullptr;
    textureEmission = nullptr;
    textureFlags = 0;
}

void MeshRenderer::Render(glm::mat4 transform) {

    textureFlags =
        (textureDiffuse != nullptr) +
        ((textureSpecular != nullptr) << 1) +
        ((textureEmission != nullptr) << 2) +
        ((Skybox::current != nullptr) << 3);

    if (mesh == nullptr) {

        mesh = VertexData::GetPrefab(0);
        shader = Shader::billboardShader;
    }
    glm::mat4 view = ViewProvider::GetCurrent()->GetViewMatrix();

    if (Shader::currentShader != shader)
        shader->Use();

    if (material != nullptr) {
        shader->SetVec3("surface.ambient", material->ambient);
        shader->SetVec3("surface.diffuse", material->diffuse);
        shader->SetVec3("surface.specular", material->specular);
        shader->SetFloat("surface.shininess", material->shininess);
        shader->SetVec3("color", material->ambient);
    }
    shader->SetInt("textureFlags", textureFlags);

    auto projection = ViewProvider::GetCurrent()->GetProjection();
    shader->SetMat4("projection", projection);
    shader->SetMat4("view", view);
    shader->SetMat4("transform", transform);

    if (textureDiffuse)
    {
        shader->SetInt("textureD", 0);
        shader->SetVec4("textureD_SO", scaleOffsetDiffuse);
        textureDiffuse->Bind(0);
    }
    if (textureSpecular)
    {
        shader->SetInt("textureS", 1);
        shader->SetVec4("textureS_SO", scaleOffsetSpecular);
        textureSpecular->Bind(1);
    }
    if (textureEmission)
    {
        shader->SetInt("textureA", 2);
        shader->SetVec4("textureA_SO", scaleOffsetEmission);
        textureEmission->Bind(2);
    }

    if (Skybox::current != nullptr)
    {
        shader->SetInt("skyTexture", 3);
        Skybox::current->cubemap->Bind(3);
    }
    mesh->Draw();

    if (textureEmission) Texture2D::Unbind(2);
    if (textureSpecular) Texture2D::Unbind(1);
    if (textureDiffuse) Texture2D::Unbind(0);
}

void MeshRenderer::setTexture(Texture2D* tex, int texBit)
{
    if ((texBit & 1) != 0) textureDiffuse = tex;
    if ((texBit & 2) != 0) textureSpecular = tex;
    if ((texBit & 4) != 0) textureEmission = tex;
}
void MeshRenderer::setTexture(const std::string& path)
{
    textureDiffuse = new Texture2D();
    textureDiffuse->Load(path);
}

void MeshRenderer::InitialiseRenderer() {
    defaultRenderer = new MeshRenderer(VertexData::GetPrefab(VD_RECTANGLE), Shader::billboardShader);
    defaultRenderer->setTexture(assetPath + "/default.png");
    skyboxRenderer = new MeshRenderer(VertexData::GetPrefab(VD_RECTANGLE), Shader::skysphereShader);
    skyboxRenderer->setTexture(assetPath + "/skybox.jpg");
}