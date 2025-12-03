#include "tetrapc.h"
#include "Application.h"

#include "utils/Time.h"

using namespace TetraEngine;

Application::Application() {
    updateManager = std::make_unique<UpdateManager>();
}

void Application::Update() {
    updateManager->UpdateAll(Time::deltaTime);
}

void Application::DrawGUI(ImVec2 origin, ImVec2 size) {

}

void Application::ToggleCursor(bool state) {
    cursorEnabled = state;
}

bool Application::IsCursorEnabled() {
    return cursorEnabled;
}
