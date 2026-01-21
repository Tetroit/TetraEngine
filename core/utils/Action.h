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
        std::vector<std::pair<std::string, Function>> toAdd;
        std::vector<std::string> toRemove;
        bool locked = false;

    public:
        void AddCallback(Function func, std::string name);
        void RemoveCallback(std::string name);
        void Call(Args... args);
        bool IsEmpty();
    };

    template<typename ... Args>
    void Action<Args...>::AddCallback(Function func, std::string name) {
        if (locked) {
            toAdd.emplace_back(name, func);
        }
        else {
            callbacks[name] = func;
        }
    }

    template<typename ... Args>
    void Action<Args...>::RemoveCallback(std::string name) {
        if (locked) {
            toRemove.emplace_back(name);
        }
        else {
            callbacks.erase(name);
        }
    }

    template<typename ... Args>
    void Action<Args...>::Call(Args... args) {
        if (IsEmpty()) {
            return;
        }
        locked = true;
        for (auto& [name, func] : callbacks){
            std::cout << "Invoked " << name << std::endl;
            func(args...);
        }
        for (auto& name : toRemove) {
            callbacks.erase(name);
        }
        for (auto& [name, func] : toAdd) {
            callbacks.emplace(name, func);
        }
        locked = false;
        toAdd.clear();
        toRemove.clear();
    }

    template<typename ... Args>
    bool Action<Args...>::IsEmpty() {
        return callbacks.empty();
    }
} // TetraEngine
