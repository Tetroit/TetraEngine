#include "PhysXUtils.h"
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include "glm/gtx/quaternion.hpp"

namespace TetraEngine {
    physx::PxVec3 PhysXUtils::Vec3ToPX(const glm::vec3 &v) {
        return physx::PxVec3(v.x, v.y, v.z);
    }

    physx::PxVec4 PhysXUtils::Vec4ToPX(const glm::vec4 &v) {
        return physx::PxVec4(v.x, v.y, v.z, v.w);
    }

    physx::PxQuat PhysXUtils::QuatToPX(const glm::quat &q) {
        return physx::PxQuat(q.x, q.y, q.z, q.w);
    }

    physx::PxTransform PhysXUtils::TransformToPX(const glm::mat4 &m) {
        glm::vec3 translation(m[3]);
        glm::mat3 rotation(m);
        glm::vec3 scale;
        rotation[0] /= glm::length(rotation[0]);
        rotation[1] /= glm::length(rotation[1]);
        rotation[2] /= glm::length(rotation[2]);
        auto rot = glm::quat_cast(rotation);
        return physx::PxTransform(Vec3ToPX(translation), QuatToPX(rot));
    }

    glm::vec3 PhysXUtils::PXToVec3(const physx::PxVec3 &v) {
        return glm::vec3(v.x, v.y, v.z);
    }
    glm::vec4 PhysXUtils::PXToVec4(const physx::PxVec4 &v) {
        return glm::vec4(v.x, v.y, v.z, v.w);
    }
    glm::quat PhysXUtils::PXToQuat(const physx::PxQuat &v) {
        return glm::quat(v.w, v.x, v.y, v.z);
    }

    glm::mat4 PhysXUtils::PXToTransform(const physx::PxTransform &m) {

        auto rot = glm::toMat4(PXToQuat(m.q));
        glm::mat4 transform(1);
        transform *= rot;
        transform = glm::translate(transform, PXToVec3(m.p));
        return transform;
    }
} // TetraEngine