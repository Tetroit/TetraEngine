//
// Created by pchyo on 21/11/2025.
//

#include "ViewProvider.h"

namespace TetraEngine {

    ViewProvider* ViewProvider::current = nullptr;

    ViewProvider * ViewProvider::GetCurrent() {
        return current;
    }

    void ViewProvider::SetCurrent(ViewProvider* pCurrent) {
        if (current != nullptr) {
            current->Disable();
        }
        if (pCurrent!= nullptr) {
            pCurrent->Enable();
        }
        current = pCurrent;
    }
} // TetraEngine