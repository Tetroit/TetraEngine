#pragma once
#include "Entity.h"

namespace TetraEngine {
    namespace ECS {

        template <class T>
        struct Handle {

            static Handle CreateInvalid();

            uint slot = std::numeric_limits<uint>::max();
            uint compGeneration = std::numeric_limits<uint>::max();

            Handle() = default;
            Handle(uint slot, uint gen) : slot(slot), compGeneration(gen) {}
            bool Valid() const;
            void Invalidate();
            bool operator==(const Handle& other) const;
        };

        template<class T>
        Handle<T> Handle<T>::CreateInvalid() {
            return Handle(std::numeric_limits<uint>::max(), 0);
        }

        template<class T>
        bool Handle<T>::Valid() const { return slot != std::numeric_limits<uint>::max();}

        template<class T>
        void Handle<T>::Invalidate() {
            slot = std::numeric_limits<uint>::max();
        }

        template<class T>
        bool Handle<T>::operator==(const Handle &other) const {
            return slot == other.slot && compGeneration == other.compGeneration;
        }
    } // ECS
} // TetraEngine
