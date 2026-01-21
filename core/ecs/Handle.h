#pragma once
#include "Entity.h"

namespace TetraEngine {
    namespace ECS {

        struct TypeErasedHandle;

        template <class T>
        struct Handle {

            static constexpr Handle CreateInvalid();

            uint slot = std::numeric_limits<uint>::max();
            uint compGeneration = std::numeric_limits<uint>::max();

            Handle() = default;
            Handle(uint slot, uint gen) : slot(slot), compGeneration(gen) {}
            Handle(const TypeErasedHandle& other);

            bool Valid() const;
            void Invalidate();
            bool operator==(const Handle& other) const;
        };
        template<class T>
        struct HandleHasher
        {
            size_t operator()(const Handle<T>& h) const noexcept
            {
                return (static_cast<size_t>(h.slot) << 32) |
                       static_cast<size_t>(h.compGeneration);
            }
        };
        struct TypeErasedHandle {
            uint type;
            uint slot = std::numeric_limits<uint>::max();
            uint compGeneration = std::numeric_limits<uint>::max();
            template<class T>
            TypeErasedHandle(const Handle<T>& handle);
        };

        template<class T>
        constexpr Handle<T> Handle<T>::CreateInvalid() {
            return Handle(std::numeric_limits<uint>::max(), 0);
        }

        template<class T>
        Handle<T>::Handle(const TypeErasedHandle &other) : Handle(other.slot, other.compGeneration){};

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

        template<class T>
        TypeErasedHandle::TypeErasedHandle(const Handle<T> &handle) :
        slot(handle.slot),
        compGeneration(handle.compGeneration),
        type(static_cast<uint>(typeid(T).hash_code())) {}
    } // ECS
} // TetraEngine
