#pragma once
#include "foundation/PxErrorCallback.h"

namespace TetraEngine {
    class PhysXErrorCallback : public physx::PxErrorCallback{
        void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
    };
} // TetraEngine