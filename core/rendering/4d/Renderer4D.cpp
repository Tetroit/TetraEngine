
#include "Renderer4D.h"

#include "../Shader.h"
#include "../ViewProvider.h"
#include "../../utils/ComputeShaderLoader.h"
#include "../Material.h"
#include "../Skybox.h"

namespace TetraEngine {
    void Renderer4D::SetupBuffers() {
        if (VAO == 0) {
            glGenVertexArrays(1, &VAO);
        }
        if (VBO == 0) {
            glGenBuffers(1, &VBO);
        }
        if (EBO == 0) {
            glGenBuffers(1, &EBO);
        }
        if (vertBufferSSBO == 0) {
            glGenBuffers(1, &vertBufferSSBO);
        }
        if (faceBufferSSBO == 0) {
            glGenBuffers(1, &faceBufferSSBO);
        }
        if (edgeBufferSSBO == 0) {
            glGenBuffers(1, &edgeBufferSSBO);
        }
    }

    void Renderer4D::initBuffers() {
    }

    Renderer4D::Renderer4D(Mesh4D *vd, Shader *sh) : mesh(vd), shader(sh) {
        rotationBivectors = {0,0,0,0,0,0};
        translation4D = {0,0,0,0};
        sliceW = 0.0f;
        SetupBuffers();
    }

    void Renderer4D::ComputeEdges() {

        if (edgeComputeShader == 0) {
            edgeComputeShader = ComputeShaderLoader::loadCompute(shaderPath + "/tetrahedronEdges.comp");
        }
        initBuffers();
        auto vData = mesh->GetVertices();
        auto fData = mesh->GetIndices();
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, vertBufferSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER,vData.size() * sizeof(Vertex4D),vData.data(),GL_STATIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertBufferSSBO);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, faceBufferSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER,fData.size() * sizeof(uint),fData.data(),GL_STATIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, faceBufferSSBO);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, edgeBufferSSBO);
        //per each tetra: 4 vertices, 6 edges (12 edge ends) -> 12/4 = 3
        glBufferData(GL_SHADER_STORAGE_BUFFER,fData.size()*3 * sizeof(glm::vec4),nullptr,GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, edgeBufferSSBO);

        glUseProgram(edgeComputeShader);
        const int groupSize = 256;
        glDispatchCompute((fData.size()/4 + groupSize - 1) / groupSize, 1, 1);

        glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
    }
    void Renderer4D::Render(ViewProvider *viewProvider, glm::mat4 transformMat) {
        if (mesh == nullptr)
            LOG_ERR_FROM("4D RENDERER", "No mesh was attached while rendering");

        auto proj = viewProvider->GetProjection();
        auto view = viewProvider->GetViewMatrix();
        UpdateRotationMatrix();
        shader->Use();
        shader->SetMat4("projection", proj);
        shader->SetMat4("view", view);
        shader->SetMat4("transform", transformMat);
        shader->SetFloat("sliceW", sliceW);

        shader->SetMat4("rotation4D", rotation4D);
        shader->SetVec4("translation4D", translation4D);

        if (material != nullptr) {
            shader->SetVec3("surface.ambient", material->ambient);
            shader->SetVec3("surface.diffuse", material->diffuse);
            shader->SetVec3("surface.specular", material->specular);
            shader->SetFloat("surface.shininess", material->shininess);
            shader->SetVec3("color", material->ambient);
        }
        textureFlags =
            ((Skybox::current != nullptr) << 3);

        if (Skybox::current != nullptr)
        {
            shader->SetInt("skyTexture", 3);
            Skybox::current->cubemap->Bind(3);
        }

        SetupBuffers();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->GetVertices().size() * sizeof(Vertex4D), mesh->GetVertices().data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndices().size() * sizeof(uint32_t), mesh->GetIndices().data(), GL_DYNAMIC_DRAW);


        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex4D), (void*)nullptr);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex4D), (void*)(4 * sizeof(float)));
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex4D), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glDrawElements(GL_LINES_ADJACENCY, mesh->GetIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        if (renderWireframe) {

            int tetraCount = mesh->GetIndices().size()/4;
            ComputeEdges();
            wireframeShader->Use();
            wireframeShader->SetMat4("projection", proj);
            wireframeShader->SetMat4("view", view);
            wireframeShader->SetMat4("transform", transformMat);
            wireframeShader->SetFloat("minW", GetMinW());
            wireframeShader->SetFloat("maxW", GetMaxW());

            wireframeShader->SetMat4("rotation4D", rotation4D);
            wireframeShader->SetVec4("translation4D", translation4D);

            static GLuint WireVAO, WireVBO;
            if (WireVAO == 0) {
                glGenVertexArrays(1, &WireVAO);
            }
            if (WireVBO == 0) {
                glGenBuffers(1, &WireVBO);
            }

            glBindVertexArray(WireVAO);
            glBindBuffer(GL_ARRAY_BUFFER, edgeBufferSSBO);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)nullptr);
            glDrawArrays(GL_LINES, 0, tetraCount * 12);
            glBindVertexArray(0);
        }
    }

    void Renderer4D::SetSliceW(float newSlice) {
        sliceW = newSlice;
    }

    void Renderer4D::SetMesh(Mesh4D *mesh) {
        this->mesh = mesh;
    }

    float Renderer4D::GetMinW() {
        float min = std::numeric_limits<float>::max();
        for (const auto& tetra : mesh->GetTetrahedrons()) {
            for (auto i : tetra.v) {
                min = std::min(i.w, min);
            }
        }
        return min;
    }
    float Renderer4D::GetMaxW() {
        float max = std::numeric_limits<float>::min();
        for (const auto& tetra : mesh->GetTetrahedrons()) {
            for (auto i : tetra.v) {
                max = std::max(i.w, max);
            }
        }
        return max;
    }

    void Renderer4D::UpdateRotationMatrix() {
        std::array<float, 6> sin{};
        std::array<float, 6> cos{};
        for (int i = 0; i < 6; i++) {
            sin[i] = sinf(rotationBivectors[i]);
            cos[i] = cosf(rotationBivectors[i]);
        }
        rotation4D = glm::mat4(1);
        rotation4D = ConstructRotationOnPlane(0,1,sin[0],cos[0]) *
             ConstructRotationOnPlane(0,2,sin[1],cos[1]) *
             ConstructRotationOnPlane(0,3,sin[2],cos[2]) *
             ConstructRotationOnPlane(1,2,sin[3],cos[3]) *
             ConstructRotationOnPlane(1,3,sin[4],cos[4]) *
             ConstructRotationOnPlane(2,3,sin[5],cos[5]);
    }
    glm::mat4 Renderer4D::ConstructRotationOnPlane(int axis1, int axis2, float sin, float cos) {
        auto res = glm::mat4(1);
        res[axis1][axis1] = cos;
        res[axis2][axis2] = cos;
        res[axis1][axis2] = -sin;
        res[axis2][axis1] = sin;
        return res;
    }

    std::vector<glm::vec4> Renderer4D::GetWireframe() {
        std::vector<glm::vec4> wireframe;
        for (const auto& tetra: mesh->GetTetrahedrons()) {
            for (int i=0; i<4; i++) {
                for (int j=0; j<i; j++) {
                    wireframe.push_back(tetra.v[i]);
                    wireframe.push_back(tetra.v[j]);
                }
            }
        }
        return wireframe;
    }
}
