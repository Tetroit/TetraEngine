#pragma once

namespace TetraEngine {
    template<class T>
    class Brush {
        T* context = nullptr;
    public:
        explicit Brush(T* context = nullptr) : context(context) {}
        void Set(T* newContext);
        void Clear();
    };

    template<class T>
    void Brush<T>::Set(T *newContext) {
        context = newContext;
    }

    template<class T>
    void Brush<T>::Clear() {
        context = nullptr;
    }
}
