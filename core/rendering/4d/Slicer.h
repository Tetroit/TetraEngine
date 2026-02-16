#pragma once
#include "tetrapc.h"

namespace TetraEngine {
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
        static std::vector<glm::vec3> slice4D(const std::vector<glm::vec4[4]> & tetras, glm::vec4 planeNormal, float planeOffset);
    };
} // TetraEngine