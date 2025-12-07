#include "tetrapc.h"
#include "InputManager.h"
#include "FreeType.h"

#include "Core.h"

using namespace TetraEngine;


#ifdef TETRA_GLFW

const InputManager::keyArray InputManager::keyNames = {
	{
		{32, "Space"},
		{39, "Apostrophe"},
		{44, "Comma"},
		{45, "Minus"},
		{46, "Period"},
		{47, "Slash"},
		{48, "0"},
		{49, "1"},
		{50, "2"},
		{51, "3"},
		{52, "4"},
		{53, "5"},
		{54, "6"},
		{55, "7"},
		{56, "8"},
		{57, "9"},
		{59, "Semicolon"},
		{61, "Equal"},
		{65, "A"},
		{66, "B"},
		{67, "C"},
		{68, "D"},
		{69, "E"},
		{70, "F"},
		{71, "G"},
		{72, "H"},
		{73, "I"},
		{74, "J"},
		{75, "K"},
		{76, "L"},
		{77, "M"},
		{78, "N"},
		{79, "O"},
		{80, "P"},
		{81, "Q"},
		{82, "R"},
		{83, "S"},
		{84, "T"},
		{85, "U"},
		{86, "V"},
		{87, "W"},
		{88, "X"},
		{89, "Y"},
		{90, "Z"},
		{91, "Left Bracket"},
		{92, "Backslash"},
		{93, "Right Bracket"},
		{96, "Grave Accent"},
		{256, "Escape"},
		{257, "Enter"},
		{258, "Tab"},
		{259, "Backspace"},
		{260, "Insert"},
		{261, "Delete"},
		{262, "Right"},
		{263, "Left"},
		{264, "Down"},
		{265, "Up"},
		{266, "Page Up"},
		{267, "Page Down"},
		{268, "Home"},
		{269, "End"},
		{280, "Caps Lock"},
		{281, "Scroll Lock"},
		{282, "Num Lock"},
		{283, "Print Screen"},
		{284, "Pause"},
		{290, "F1"},
		{291, "F2"},
		{292, "F3"},
		{293, "F4"},
		{294, "F5"},
		{295, "F6"},
		{296, "F7"},
		{297, "F8"},
		{298, "F9"},
		{299, "F10"},
		{300, "F11"},
		{301, "F12"},
		{302, "F13"},
		{303, "F14"},
		{304, "F15"},
		{305, "F16"},
		{306, "F17"},
		{307, "F18"},
		{308, "F19"},
		{309, "F20"},
		{310, "F21"},
		{311, "F22"},
		{312, "F23"},
		{313, "F24"},
		{314, "F25"},
		{320, "KP 0"},
		{321, "KP 1"},
		{322, "KP 2"},
		{323, "KP 3"},
		{324, "KP 4"},
		{325, "KP 5"},
		{326, "KP 6"},
		{327, "KP 7"},
		{328, "KP 8"},
		{329, "KP 9"},
		{330, "KP Decimal"},
		{331, "KP Divide"},
		{332, "KP Multiply"},
		{333, "KP Subtract"},
		{334, "KP Add"},
		{335, "KP Enter"},
		{336, "KP Equal"},
		{340, "Left Shift"},
		{341, "Left Control"},
		{342, "Left Alt"},
		{343, "Left Super"},
		{344, "Right Shift"},
		{345, "Right Control"},
		{346, "Right Alt"},
		{347, "Right Super"},
		{348, "Menu"}
	}
};
const InputManager::mbArray InputManager::mouseNames = {
	{
		{GLFW_MOUSE_BUTTON_1, "Left Mouse Button"},
		{GLFW_MOUSE_BUTTON_2, "Right Mouse Button"},
		{GLFW_MOUSE_BUTTON_3, "Middle Mouse Button"},
		{GLFW_MOUSE_BUTTON_4, "Mouse Button 4"},
		{GLFW_MOUSE_BUTTON_5, "Mouse Button 5"},
		{GLFW_MOUSE_BUTTON_6, "Mouse Button 6"},
		{GLFW_MOUSE_BUTTON_7, "Mouse Button 7"},
		{GLFW_MOUSE_BUTTON_8, "Mouse Button 8"}
	}
};

constexpr std::string_view InputManager::GetKeyName(int key) {
	for (auto [k, name] : keyNames)
		if (k == key)
			return name;
	return "Unknown";
}
constexpr std::string_view InputManager::GetMouseButtonName(int key) {
	for (auto [k, name] : mouseNames)
		if (k == key)
			return name;
	return "Unknown";
}

#endif //TETRA_GLFW

InputManager::InputManager()
{
	for (int i = 0; i < GLFWManager::key_count; i++)
		keys[i] = false;

	for (int i = 0; i < GLFWManager::mb_count; i++)
		mouseButtons[i] = false;
}

InputManager::~InputManager()
{
}


InputManager* InputManager::GetMain()
{
	return Core::inputManager;
}
void InputManager::OnKeyDown(int key)
{
	DispatchKeyEvent(key, GLFW_PRESS);
	keys[key] = true;
}
void InputManager::OnKeyUp(int key)
{
	DispatchKeyEvent(key, GLFW_RELEASE);
	keys[key] = false;
}
void InputManager::OnKeyPressed(int key)
{
	DispatchKeyEvent(key, GLFW_REPEAT);
}


void InputManager::OnMouseDown(int button)
{
	DispatchMouseEvent(button, GLFW_PRESS);
	mouseButtons[button] = true;
}
void InputManager::OnMouseUp(int button)
{
	DispatchMouseEvent(button, GLFW_RELEASE);
	mouseButtons[button] = false;
}
void InputManager::OnMousePressed(int button)
{
	DispatchMouseEvent(button, GLFW_REPEAT);
}

void InputManager::OnMouseMove()
{
	DispatchMouseMoveEvent();
}


void InputManager::Update() {
	UpdateKeys();
	UpdateMouse();
}
void InputManager::UpdateKeys()
{
	pressedKeys = "";
	anyPressed = false;
	for (int i = 0; i < GLFWManager::key_count; i++)
	{
		if (keys[i])
		{
			if (pressedKeys.size() == 0)
				pressedKeys.append(GetKeyName(i));
			else {
				pressedKeys.append(" & ");
				pressedKeys.append(GetKeyName(i));
			}
			OnKeyPressed(i);
			anyPressed = true;
		}
	}
}
void InputManager::UpdateMouse()
{
	anyPressed = false;
	for (int i = 0; i < GLFWManager::mb_count; i++)
	{
		if (mouseButtons[i])
		{
			OnMousePressed(i);
			anyPressed = true;
		}
	}
}
void InputManager::UpdateMouseMovement(float newX, float newY)
{
	deltaMouseX = newX - mousePosX;
	deltaMouseY = newY - mousePosY;
	mousePosX = newX;
	mousePosY = newY;
	OnMouseMove();
}

EventDispatcher<InputEvent> & InputManager::GetGameDispatcher() {
	return gameDispatcher;
}

EventDispatcher<InputEvent> & InputManager::GetEditorDispatcher() {
	return editorDispatcher;
}

#ifdef TETRA_GLFW

void InputManager::DispatchKeyEvent(int key, int mode)
{
	std::string name = std::string(GetKeyName(key));

	gameDispatcher.Invoke(KeyEvent(key, mode));
	gameDispatcher.Invoke(AnyKeyEvent(key, mode));
}

void InputManager::DispatchMouseEvent(int key, int mode)
{
	gameDispatcher.Invoke(MouseEvent(key, mode));
	gameDispatcher.Invoke(AnyMouseEvent(key, mode));
}
void InputManager::DispatchMouseMoveEvent()
{
	std::string name = "Mouse moved";
	gameDispatcher.Invoke(MouseMoveEvent(deltaMouseX, deltaMouseY, mousePosX, mousePosY, name));

	//std::cout << name << '\n';
	//std::cout << deltaMouseX << " " << deltaMouseY << " " << mousePosX << " " << mousePosY << '\n';
}

#endif // TETRA_GLFW
