#pragma once

namespace TetraEngine {
    class Application;
    class GLFWManager;
    class ImGuiManager;
    class InputManager;
    class Viewport;
    class DestroyManager;
    class Editor;
    class TypeRegistry;
    class SharedContentRegistry;
    namespace ECS {
        class ECS;
    }
    struct GlobalContext {

        Application* application = nullptr;
        GLFWManager* glfwManager = nullptr;
        ImGuiManager* imguiManager = nullptr;
        InputManager* inputManager = nullptr;
        Viewport* mainViewport = nullptr;
        DestroyManager* destroyManager = nullptr;
        Editor* editor = nullptr;
        TypeRegistry* typeRegistry = nullptr;
        SharedContentRegistry* resources = nullptr;
        ECS::ECS* ecs = nullptr;

        float lastMouseX;
        float lastMouseY;
        bool cursorEnabled = false;
    };
}