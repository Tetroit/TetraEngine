
#include "PhysXErrorCallback.h"

namespace TetraEngine {
    void PhysXErrorCallback::reportError(physx::PxErrorCode::Enum code, const char *message, const char *file,
        int line) {
        printf("PhysX Error [%d] in %s:%d: %s\n", code, file, line, message);
    }
} // TetraEngine