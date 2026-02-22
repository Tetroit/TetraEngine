
#include "Renderer4D.h"

#include "../Shader.h"
#include "../ViewProvider.h"

namespace TetraEngine {
    void Renderer4D::SetupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    Renderer4D::Renderer4D(Mesh4D *vd, Shader *sh) : mesh(vd), shader(sh) {
        planeNormal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0);
        normalOffset = 0.0f;
        SetupBuffers();
    }

    void Renderer4D::Render(ViewProvider *viewProvider, glm::mat4 transformMat) {
        auto proj = viewProvider->GetProjection();
        auto view = viewProvider->GetViewMatrix();

        shader->Use();
        shader->SetMat4("projection", proj);
        shader->SetMat4("view", view);
        shader->SetMat4("transform", transformMat);

        if (VAO == 0 || VBO == 0) {
            SetupBuffers();
        }
        auto sliced = Slicer::slice4D(mesh->GetTetrahedrons(), planeNormal, normalOffset);
        glBindVertexArray(VAO);

        if (!sliced.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sliced.size() * sizeof(glm::vec3), sliced.data(), GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
            glEnableVertexAttribArray(0);

            glDrawArrays(GL_TRIANGLES, 0, sliced.size());
            glBindVertexArray(0);
        }
        if (renderWireframe) {

            wireframeShader->Use();
            wireframeShader->SetMat4("projection", proj);
            wireframeShader->SetMat4("view", view);
            wireframeShader->SetMat4("transform", transformMat);
            wireframeShader->SetFloat("minW", GetMinW());
            wireframeShader->SetFloat("maxW", GetMaxW());

            auto wireframe = GetWireframe();
            static GLuint WireVAO, WireVBO;
            glGenVertexArrays(1, &WireVAO);
            glGenBuffers(1, &WireVBO);

            glBindVertexArray(WireVAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, wireframe.size() * sizeof(glm::vec4), wireframe.data(), GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
            glDrawArrays(GL_LINES, 0, wireframe.size());
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
} // TetraEngine