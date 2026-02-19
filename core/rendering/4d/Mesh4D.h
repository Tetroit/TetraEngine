#pragma once
#include <vector>

#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

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
    };
    class Mesh4D {
        std::vector<Tetrahedron4D> tetras;
        // std::vector<Vertex4D> vertices;
        // std::vector<glm::u32vec4> tetrahedrons;
        // std::vector<glm::vec2> texcoords;
        // std::vector<glm::vec4> normals;
    public:
        const std::vector<Tetrahedron4D>& GetTetrahedrons();
        void AddTetrahedron(Tetrahedron4D t);
    };
} // TetraEngine