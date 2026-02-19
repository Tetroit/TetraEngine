
#include "Mesh4D.h"

namespace TetraEngine {
    const std::vector<Tetrahedron4D> & Mesh4D::GetTetrahedrons() {
        return tetras;
    }

    void Mesh4D::AddTetrahedron(Tetrahedron4D t) {
        tetras.push_back(t);
    }
} // TetraEngine