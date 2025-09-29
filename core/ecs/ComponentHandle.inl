#pragma once
#include "ComponentManager.h"

namespace  TetraEngine {
    template<class T>
    ComponentHandle<T>::ComponentHandle(Component<T> *component): component(component) {
        if (component == nullptr) {
            isValid = false;
            manager = nullptr;
            entityID = 0;
        } else {
            isValid = true;
            manager = component->manager;
            entityID = component->GetOwner();
        }
    }

    template<class T>
    void ComponentHandle<T>::Validate() {
        if (!isValid) {
            return;
        }
        if (manager->GetComponent(ComponentManager::template GetTypeId<T>(), entityID) == nullptr){
            isValid = false;
        }
    }

    template<class T>
    Component<T>* ComponentHandle<T>::operator->() {
        Validate();
        if (!isValid) {
            return nullptr;
        }
        return component;
    }

    template<class T>
    ComponentHandle<T>::operator bool() {
        Validate();
        return isValid;
    }
}
