#include "Slicer.h"

#include "Mesh4D.h"

namespace TetraEngine {
    std::vector<glm::vec3> Slicer::slice4D(const std::vector<Tetrahedron4D> &tetras, glm::vec4 planeNormal, float planeOffset) {
        std::vector<glm::vec3> result;
        glm::vec4 planeOrigin = planeNormal * planeOffset;
        for (const auto& tetra: tetras) {
            bool isAbovePlane[4];
            std::vector<int> above, below;
            for (int i = 0; i < 4; i++) {
                glm::vec4 vert = tetra.v[i];
                float fac = glm::dot(vert, planeNormal) - planeOffset;
                if (fac >= 0) {
                    isAbovePlane[i] = true;
                    above.push_back(i);
                }
                else {
                    below.push_back(i);
                }
            }
            if (above.empty() || below.empty()) continue;
            std::vector<std::pair<int, int>> edgeIndex;
            std::vector<std::pair<glm::vec4, glm::vec4>> edgesToSplit;
            std::vector<int>& lowCnt = above.size() >= below.size() ? below : above;
            std::vector<int>& highCnt = above.size() >= below.size() ? above : below;
            std::vector<glm::vec4> sects;
            bool flipNormal = above.size() >= below.size() ? false : true;
            if (lowCnt.size() == 1) {
                int baseID = lowCnt[0];
                for (int i=0; i<3; i++) {
                    edgeIndex.emplace_back(baseID, vertOrderTri[baseID][i]);
                }
            }
            else if (lowCnt.size() == 2) {
                if (!isAbovePlane[0]) {
                    lowCnt = above;
                    highCnt = below;
                    flipNormal = true;
                }
                int neighbour = lowCnt[1];
                int* cycle = vertOrderQuad[neighbour];
                edgeIndex.emplace_back(cycle[0], cycle[1]);
                edgeIndex.emplace_back(cycle[2], cycle[1]);
                edgeIndex.emplace_back(cycle[2], cycle[3]);
                edgeIndex.emplace_back(cycle[0], cycle[3]);
            }
            for (const auto& edge: edgeIndex) {
                sects.emplace_back(sectSegment4D(tetra.v[edge.second], tetra.v[edge.first], planeNormal, planeOrigin));
            }
            if (sects.size() == 3) {
                for (int i=0; i<3; i++) {
                    result.emplace_back(sects[flipNormal ? 2-i : i]);
                }
            }
            if (sects.size() == 4) {
                if (!flipNormal) {
                    result.emplace_back(sects[0]);
                    result.emplace_back(sects[1]);
                    result.emplace_back(sects[2]);
                    result.emplace_back(sects[0]);
                    result.emplace_back(sects[2]);
                    result.emplace_back(sects[3]);
                }
                else {
                    result.emplace_back(sects[2]);
                    result.emplace_back(sects[1]);
                    result.emplace_back(sects[0]);
                    result.emplace_back(sects[3]);
                    result.emplace_back(sects[2]);
                    result.emplace_back(sects[0]);
                }
            }
        }
        return result;
    }

    glm::vec4 Slicer::sectSegment4D(const glm::vec4 &a, const glm::vec4 &b, const glm::vec4 &planeNormal,
        const glm::vec4 &planeOrigin) {
        glm::vec4 l = b - a;
        glm::vec4 p = planeOrigin - a;
        float t = glm::dot(planeNormal, p)/glm::dot(planeNormal, l);
        return a + t * l;
    }
}
