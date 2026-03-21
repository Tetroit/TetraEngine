
#include "Mesh4D.h"

#include <array>

#include "Slicer.h"

namespace TetraEngine {
    const std::vector<Tetrahedron4D> & Mesh4D::GetTetrahedrons() {
        return tetras;
    }
    void Mesh4D::AddTetrahedron(Tetrahedron4D t) {
        tetras.push_back(t);
    }
    void Mesh4D::AddTetrahedron(std::array<uint,4> ids) {
        indices.insert(indices.end(), ids.begin(), ids.end());
    }
    void Mesh4D::AddTetrahedrons(std::vector<uint> ids) {
        assert(ids.size() % 4 == 0);
        indices.insert(indices.end(), ids.begin(), ids.end());
    }

    uint Mesh4D::AddVertex(Vertex4D v) {
        vertices.push_back(v);
        return vertices.size() - 1;
    }

    uint Mesh4D::AddVertices(std::vector<Vertex4D> v) {
        uint firstID = vertices.size();
        vertices.insert(vertices.end(), v.begin(), v.end());
        return firstID;
    }


    void Mesh4D::AddCuboid(glm::vec4 corner, glm::vec4 a, glm::vec4 b, glm::vec4 c) {
        glm::vec4 v000 = corner;
        glm::vec4 v100 = v000+a;
        glm::vec4 v010 = v000+b;
        glm::vec4 v001 = v000+c;
        glm::vec4 v110 = v100+b;
        glm::vec4 v101 = v100+c;
        glm::vec4 v011 = v010+c;
        glm::vec4 v111 = v110+c;
        tetras.emplace_back(v000,v100,v010,v001);
        tetras.emplace_back(v110,v010,v100,v111);
        tetras.emplace_back(v101,v100,v001,v111);
        tetras.emplace_back(v011,v001,v010,v111);
        tetras.emplace_back(v111,v001,v010,v100);

        glm::vec4 normal = glm::normalize(Slicer::cross4D(a,b,c));

        uint start = AddVertices({
            corner,
            corner+c,
            corner+b,
            corner+b+c,
            corner+a,
            corner+a+c,
            corner+a+b,
            corner+a+b+c,
        });
        for (int i=start; i<start+8; i++) {
            vertices[i].normal = normal;
        }
        AddTetrahedrons({
            start + 0b000, start + 0b100, start + 0b010, start + 0b001,
            start + 0b110, start + 0b010, start + 0b100, start + 0b111,
            start + 0b101, start + 0b100, start + 0b001, start + 0b111,
            start + 0b011, start + 0b001, start + 0b010, start + 0b111,
            start + 0b111, start + 0b001, start + 0b010, start + 0b100
        });
    }

    std::shared_ptr<Mesh4D> Mesh4D::MakeTesseract(glm::vec4 pos, glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d) {
        auto mesh = std::make_shared<Mesh4D>();
        mesh->AddCuboid(pos, a, b, c);
        mesh->AddCuboid(pos, d, c, b);
        mesh->AddCuboid(pos, a, c, d);
        mesh->AddCuboid(pos, a, d, b);
        mesh->AddCuboid(pos + a, b, c, d);
        mesh->AddCuboid(pos + b, a, d, c);
        mesh->AddCuboid(pos + c, a, b, d);
        mesh->AddCuboid(pos + d, c, b, a);

        return mesh;
    }
}
