#pragma once
#include "Mesh4D.h"
#include "Slicer.h"

namespace TetraEngine {
    class Shader;
    class ViewProvider;

    class Renderer4D {
        glm::vec4 planeNormal;
        float normalOffset;
        Mesh4D* mesh;

        GLuint VAO, VBO;
        void SetupBuffers();
    public:
        Renderer4D(Mesh4D* vd, Shader* sh);
        Shader* shader;
        void Render(ViewProvider* viewProvider, glm::mat4 transformMat = glm::mat4(1.0f));
        void SetSectionPlaneOffset(float newOffset);
        float GetSectionPlaneOffset();
        void SetSectionPlanePosition(glm::vec4 newNormal);
        void SetMesh(Mesh4D* mesh);
        glm::vec4 GetSectionPlanePostion();
    };
}
