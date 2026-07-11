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

        Application* application;
        GLFWManager* glfwManager;
        ImGuiManager* imguiManager;
        InputManager* inputManager;
        Viewport* mainViewport;
        DestroyManager* destroyManager;
        Editor* editor;
        TypeRegistry* typeRegistry;
        SharedContentRegistry* resources;
        ECS::ECS* ecs;

        float lastMouseX;
        float lastMouseY;
        bool cursorEnabled = false;
    };
}