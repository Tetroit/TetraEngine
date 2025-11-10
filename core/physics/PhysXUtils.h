#pragma once

#include "tetrapc.h"
#include <PxPhysicsAPI.h>

namespace TetraEngine {
    class PhysXUtils {

    public:
        [[nodiscard]] static physx::PxVec3 Vec3ToPX(const glm::vec3& v);
        [[nodiscard]] static physx::PxVec4 Vec4ToPX(const glm::vec4& v);
        [[nodiscard]] static physx::PxQuat QuatToPX(const glm::quat& q);
        [[nodiscard]] static physx::PxTransform TransformToPX(const glm::mat4& m);

        [[nodiscard]] static glm::vec3 PXToVec3(const physx::PxVec3& v);
        [[nodiscard]] static glm::vec4 PXToVec4(const physx::PxVec4& v);
        [[nodiscard]] static glm::quat PXToQuat(const physx::PxQuat& q);
        [[nodiscard]] static glm::mat4 PXToTransform(const physx::PxTransform& m);
    };
} // TetraEngine
