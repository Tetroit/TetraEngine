
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
        auto view = viewProvider->GetProjection();

        shader->Use();
        shader->SetMat4("projection", proj);
        shader->SetMat4("view", view);
        shader->SetMat4("transform", transformMat);

        if (VAO == 0 || VBO == 0) {
            SetupBuffers();
        }
        auto sliced = Slicer::slice4D(mesh->GetTetrahedrons(), planeNormal, normalOffset);
        glBindVertexArray(VAO);

        if (sliced.size() == 0)
            std::cout << "VERTEX BUFFER IS EMPTY";
        else {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sliced.size() * sizeof(glm::vec3), sliced.data(), GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_LINES, 0, sliced.size());
        glBindVertexArray(0);
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

    }

    glm::vec4 Renderer4D::GetSectionPlanePostion() {
        return planeNormal * normalOffset;
    }
} // TetraEngine