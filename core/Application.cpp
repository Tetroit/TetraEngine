#include "tetrapc.h"
#include "Application.h"

using namespace TetraEngine;

void Application::Update() {

}

void Application::DrawGUI(ImVec2 origin, ImVec2 size) {

}

void Application::ToggleCursor(bool state) {
    cursorEnabled = state;
}

bool Application::IsCursorEnabled() {
    return cursorEnabled;
}
