#pragma once
#include "tetrapc.h"

namespace TetraEngine {

    template<class T>
    class Component;
    class ComponentManager;

    template<class T>
    class ComponentHandle {

        bool isValid;
        ComponentManager* manager;
        uint entityID;
        Component<T>* component;

    public:
        ComponentHandle(Component<T>* component);
        void Validate();
        Component<T>* operator->();
        operator bool();
    };
} // TetraEngine

#include "ComponentHandle.inl"