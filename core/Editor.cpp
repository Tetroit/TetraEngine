#include "tetrapc.h"
#include "Editor.h"

#include <any>

#include "Core.h"
#include "rendering/Scene.h"

namespace TetraEngine {
    Editor::Editor(InputManager *inputManager, ImGuiManager *imguiManager, GLFWManager *glfwManager) :
    inputManager(inputManager),
    imguiManager(imguiManager),
    glfwManager(glfwManager),
    mode(EDIT) {
        SwitchMode(EDIT);
        inputManager->GetEditorDispatcher().AddListener(
        AnyKeyEvent::GetLink(GLFW_PRESS), &Editor::ProcessKeyEvents, *this);
        inputManager->GetEditorDispatcher().AddListener(
        AnyMouseEvent::GetLink(GLFW_PRESS), &Editor::ProcessMousePressEvents, *this);
        inputManager->GetEditorDispatcher().AddListener(
        AnyMouseEvent::GetLink(GLFW_RELEASE), &Editor::ProcessMouseReleaseEvents, *this);
    }

    Editor::~Editor() {
        inputManager->GetEditorDispatcher().RemoveListener(
        AnyKeyEvent::GetLink(GLFW_PRESS), &Editor::ProcessKeyEvents, *this);
        inputManager->GetEditorDispatcher().RemoveListener(
        AnyMouseEvent::GetLink(GLFW_PRESS), &Editor::ProcessMousePressEvents, *this);
        inputManager->GetEditorDispatcher().RemoveListener(
        AnyMouseEvent::GetLink(GLFW_RELEASE), &Editor::ProcessMouseReleaseEvents, *this);
    }

    void Editor::SwitchMode(Mode pMode) {
        switch(pMode) {
            case EDIT:
                inputManager->GetEditorDispatcher().Enable();
                inputManager->GetGameDispatcher().Disable();
                if (Scene::currentScene != nullptr) {
                    Scene::currentScene->SwitchToEditorView();
                }
                imguiManager->SetMaximize(false);
                break;
            case PLAY:
                inputManager->GetEditorDispatcher().Disable();
                inputManager->GetGameDispatcher().Enable();
                if (Scene::currentScene != nullptr) {
                    Scene::currentScene->SwitchToGameView();
                }
                imguiManager->SetMaximize(true);
                break;
        }
        mode = pMode;
    }
    void Editor::ProcessKeyEvents(const Event<InputEvent> &ev) {
        AnyKeyEvent casted = ev.ToType<AnyKeyEvent>();
        if (casted.key == GLFW_KEY_ESCAPE) {
            Core::close();
        }
        if (casted.key == GLFW_KEY_F11) {
            glfwManager->SetScreenMode(!glfwManager->IsFullscreen());
        }
    }

    void Editor::ProcessMousePressEvents(const Event<InputEvent> &ev) {
        AnyMouseEvent casted = ev.ToType<AnyMouseEvent>();
        if (casted.key == GLFW_MOUSE_BUTTON_RIGHT && imguiManager->viewportHovered) {
            glfwManager->ToggleCursor(false);
            isFocused = true;
            imguiManager->ToggleMouseEvents(false);
            LOG("MOUSE PRESS");
            std::cout << std::flush;
        }
    }
    void Editor::ProcessMouseReleaseEvents(const Event<InputEvent> &ev) {
        AnyMouseEvent casted = ev.ToType<AnyMouseEvent>();
        if (casted.key == GLFW_MOUSE_BUTTON_RIGHT) {
            glfwManager->ToggleCursor(true);
            isFocused = false;
            imguiManager->ToggleMouseEvents(true);
            LOG("MOUSE RELEASE");
            std::cout << std::flush;
        }
    }

    void Editor::Update() {
        if ((imguiManager->io->WantTextInput)
            && !isFocused && !imguiManager->viewportHovered) {
            if (glfwManager->sendKeyboardEvents)
                glfwManager->ToggleKeyboardEvents(false);
        }
        else {
            if (!glfwManager->sendKeyboardEvents)
                glfwManager->ToggleKeyboardEvents(true);
        }
        if (imguiManager->io->WantCaptureMouse && !isFocused && !imguiManager->viewportHovered)
        {
            if (glfwManager->sendMouseClickEvents)
                glfwManager->ToggleMouseClickEvents(false);
            if (glfwManager->sendMouseMoveEvents)
                glfwManager->ToggleMouseMoveEvents(false);
        }
        else
        {
            if (!glfwManager->sendMouseClickEvents)
                glfwManager->ToggleMouseClickEvents(true);
            if (!glfwManager->sendMouseMoveEvents)
                glfwManager->ToggleMouseMoveEvents(true);
        }
    }

    bool Editor::IsFocused() const {
        return isFocused;
    }

    Editor::Mode Editor::GetMode() {
        return mode;
    }
} // TetraEngine