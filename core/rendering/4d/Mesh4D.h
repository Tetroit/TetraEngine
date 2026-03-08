#pragma once
#include <variant>
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace TetraEngine {
    /// Right-handed tetrahedron
    struct Tetrahedron4D {
        Tetrahedron4D(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d) : a(a), b(b), c(c), d(d) {}
        union {
            struct {
                glm::vec4 a, b, c, d;
            };
            glm::vec4 v[4];
        };
    };
    struct Vertex4D {
        glm::vec4 position;
        glm::vec4 normal;
        glm::vec4 color;
        Vertex4D(
            glm::vec4 position,
            glm::vec4 color = {0.3f, 1.0f, 0.7f, 1.0f},
            glm::vec4 normal = glm::vec4(0.0f)) : position(position), color(color), normal(normal) {}
    };
    class Mesh4D {
        std::vector<Tetrahedron4D> tetras;
        std::vector<Vertex4D> vertices;
        std::vector<uint> indices;
    public:
        const std::vector<Tetrahedron4D>& GetTetrahedrons();
        const std::vector<Vertex4D>& GetVertices() const {return vertices;}
        const std::vector<uint>& GetIndices() const {return indices;}
        [[deprecated]] void AddTetrahedron(Tetrahedron4D t);
        void AddTetrahedron(std::array<uint, 4> ids);
        void AddTetrahedrons(std::vector<uint> ids);

        uint AddVertex(Vertex4D v);
        uint AddVertices(std::vector<Vertex4D> v);

        void AddCuboid(glm::vec4 corner, glm::vec4 a, glm::vec4 b, glm::vec4 c);
        static std::shared_ptr<Mesh4D> MakeTesseract(
            glm::vec4 pos = {-1,-1,-1,-1},
            glm::vec4 a = {2,0,0,0},
            glm::vec4 b = {0,2,0,0},
            glm::vec4 c = {0,0,2,0},
            glm::vec4 d = {0,0,0,2});
    };
} // TetraEngine