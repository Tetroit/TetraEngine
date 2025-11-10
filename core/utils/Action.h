#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <iostream>

namespace TetraEngine {

    template <typename... Args>
    class Action {

        using Function = std::function<void(Args...)>;
        std::unordered_map<std::string, Function> callbacks;

    public:
        void AddCallback(Function func, std::string name);
        void RemoveCallback(std::string name);
        void Call(Args... args);
        bool IsEmpty();
    };

    template<typename ... Args>
    void Action<Args...>::AddCallback(Function func, std::string name) {
        callbacks[name] = func;
    }

    template<typename ... Args>
    void Action<Args...>::RemoveCallback(std::string name) {
        callbacks.erase(name);
    }

    template<typename ... Args>
    void Action<Args...>::Call(Args... args) {
        if (IsEmpty()) {
            return;
        }
        for (auto& [name, func] : callbacks){
            std::cout << "Invoked " << name << std::endl;
            func(args...);
        }
    }

    template<typename ... Args>
    bool Action<Args...>::IsEmpty() {
        return callbacks.empty();
    }
} // TetraEngine
