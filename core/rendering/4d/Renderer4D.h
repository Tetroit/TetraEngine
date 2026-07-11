#pragma once
#include <array>

#include "Mesh4D.h"
#include "Slicer.h"

namespace TetraEngine {
    class Material;
    class Shader;
    class ViewProvider;

    class Renderer4D {
        float sliceW;
        Mesh4D* mesh;
        bool renderWireframe = true;

        GLuint VAO = 0, VBO = 0, EBO = 0;

        GLuint vertBufferSSBO = 0;
        GLuint faceBufferSSBO = 0;
        GLuint edgeBufferSSBO = 0;
        void SetupBuffers();
        GLuint edgeComputeShader = 0;
        std::array<float, 6> rotationBivectors;
        glm::mat4 rotation4D;
        glm::vec4 translation4D;
        int textureFlags;

        void initBuffers();
        glm::mat4 ConstructRotationOnPlane(int axis1, int axis2, float sin, float cos);

    public:
        Renderer4D(Mesh4D* vd, Shader* sh);

        Shader* shader;
        Shader* wireframeShader;
        Material* material = nullptr;

        void ComputeEdges();
        void Render(ViewProvider* viewProvider, glm::mat4 transformMat = glm::mat4(1.0f));
        void SetSliceW(float newSlice);
        void SetMesh(Mesh4D* mesh);
        float GetMinW();
        float GetMaxW();
        void UpdateRotationMatrix();
        std::vector<glm::vec4> GetWireframe();

        float* GetSliceWPtr() {return &sliceW;}
        float* GetRotationPtr(int id) {return rotationBivectors.data() + id;}
        float* GetPositionPtr() {return &translation4D[0];}
        [[nodiscard]] float GetSliceW() const {return sliceW;}
    };
}
