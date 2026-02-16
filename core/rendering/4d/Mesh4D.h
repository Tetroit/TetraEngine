#pragma once
#include <vector>

#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace TetraEngine {
    struct Vertex4D {
        glm::vec4 position;
    };
    class Mesh4D {
        std::vector<Vertex4D> vertices;
        std::vector<glm::u32vec4> tetrahedrons;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec4> normals;
    };
} // TetraEngine