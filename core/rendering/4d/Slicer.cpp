#include "Slicer.h"

namespace TetraEngine {
    std::vector<glm::vec3> Slicer::slice4D(const std::vector<glm::vec4[4]> &tetras, glm::vec4 planeNormal, float planeOffset) {
        std::vector<glm::vec3> result;
        glm::vec4 planeOrigin = planeNormal * planeOffset;
        for (const auto& tetra: tetras) {
            bool isAbovePlane[4];
            for (int i = 0; i < 4; i++) {
                glm::vec4 vert = tetra[i];
                float fac = glm::dot(vert, planeOrigin)/glm::dot(vert, vert);
                isAbovePlane[i] = fac >= 1;
            }
            std::vector<std::pair<int, int>> edgeIndex;
            std::vector<std::pair<glm::vec4, glm::vec4>> edgesToSplit;
            for (int i=0; i < 4; i++) {
                for (int j=0; j < 4; j++) {
                    if (i == j) continue;
                    if (isAbovePlane[i] && isAbovePlane[j]) continue;
                    edgesToSplit.emplace_back(tetra[i], tetra[j]);
                }
            }
            std::vector<glm::vec4> sects;
            for (std::pair edge : edgesToSplit) {
                glm::vec4 l = edge.second - edge.first;
                glm::vec4 p = planeOrigin - edge.first;
                float t = glm::dot(planeNormal, p)/glm::dot(planeNormal, l);
                sects.emplace_back(edge.first + t * l);
            }
            if (sects.size() == 3) {

            }
        }
    }
}