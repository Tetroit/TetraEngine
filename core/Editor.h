#pragma once
#include "InputManager.h"
#include "UI/ImGuiManager.h"
#include "utils/Event.h"

namespace TetraEngine {
    class InputManager;
    class ImGuiManager;
}

namespace TetraEngine {
    class Editor {
    public:
        enum Mode {
            EDIT,
            PLAY
        };
    private:
        InputManager* inputManager;
        ImGuiManager* imguiManager;
        GLFWManager* glfwManager;
        Mode mode;
        bool isFocused = false;
    public:

        Editor(
            InputManager* inputManager,
            ImGuiManager* imguiManager,
            GLFWManager* glfwManager);
        ~Editor();
        void SwitchMode(Mode mode);
        void ProcessKeyEvents(const Event<InputEvent>& ev);
        void ProcessMousePressEvents(const Event<InputEvent>& ev);

        void ProcessMouseReleaseEvents(const Event<InputEvent> &ev);
        void Update();
        bool IsFocused() const;

        Mode GetMode();
    };
} // TetraEngine