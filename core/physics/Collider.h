#pragma once

#include "tetrapc.h"
#include <PxPhysicsAPI.h>

using namespace physx;
namespace TetraEngine {

    class VertexData;
    enum ShapeType {
        BOX,
        SPHERE,
        MESH
    };

    class Collider {
        bool isTrigger = false;
        std::vector<PxShape*> shapes;
        std::vector<ShapeType> shapeTypes;
        std::unordered_map<uint, VertexData*> vertexData;

        void ResolveProperties(PxPhysics* &physics, PxMaterial* &mat);

    public:
        explicit Collider(bool isTrigger = false);
        ~Collider() = default;
        PxShape* GetShape();
        void MakeTrigger(bool state);
        [[nodiscard]] bool IsTrigger() const;

        uint AddBox(PxReal a, PxReal b, PxReal c, PxPhysics* physics = nullptr, PxMaterial* mat = nullptr);
        uint AddBox(PxReal a, PxPhysics* physics = nullptr, PxMaterial* mat = nullptr);

        uint AddSphere(PxReal r, PxPhysics* physics = nullptr, PxMaterial* mat = nullptr);
    };
} // TetraEngine