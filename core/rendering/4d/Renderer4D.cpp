
#include "Renderer4D.h"

#include "../Shader.h"
#include "../ViewProvider.h"
#include "../../utils/ComputeShaderLoader.h"

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
        planeNormal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0);
        normalOffset = 0.0f;
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

        shader->Use();
        shader->SetMat4("projection", proj);
        shader->SetMat4("view", view);
        shader->SetMat4("transform", transformMat);
        shader->SetFloat("sliceW", normalOffset);

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

    void Renderer4D::SetSectionPlaneOffset(float newOffset) {
        normalOffset = newOffset;
    }

    float Renderer4D::GetSectionPlaneOffset() {
        return normalOffset;
    }

    void Renderer4D::SetSectionPlanePosition(glm::vec4 newNormal) {
        planeNormal = glm::normalize(newNormal);
        normalOffset = glm::length(newNormal);
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

    glm::vec4 Renderer4D::GetSectionPlanePostion() {
        return planeNormal * normalOffset;
    }
}