#pragma once
#include "tetrapc.h"

namespace TetraEngine {

    struct Tetrahedron4D;


    class Slicer {
        inline static int vertOrderTri[4][3] {
            {1,2,3},
            {0,3,2},
            {0,1,3},
            {0,2,1},
        };
        inline static int vertOrderQuad[3][4] {
            {0,3,1,2}, //01 vs 23
            {0,1,2,3}, //02 vs 13
            {0,2,3,1}, //03 vs 12
        };
    public:
        static std::vector<glm::vec3> slice4D(const std::vector<Tetrahedron4D> & tetras, glm::vec4 planeNormal, float planeOffset);
        static glm::vec4 sectSegment4D(const glm::vec4& a, const glm::vec4& b, const glm::vec4& planeNormal, const glm::vec4& planeOrigin);
        static glm::vec4 cross4D(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c);
    };
} // TetraEngine