#pragma once
#include "utils/Event.h"
#include "GLFWManager.h"
#include <map>

namespace TetraEngine
{
	#ifdef TETRA_GLFW
	typedef GLFWManager::KeyInfo InputInfo;
	#endif // !TETRA_INPUT_FORMAT_DEFINED

	#ifndef TETRA_INPUT_FORMAT_DEFINED
	#error No input format found
	#endif // !TETRA_INPUTFORMAT_DEFINED

	class KeyEvent : public Event<InputInfo> {
	public:
		KeyEvent(int key, int mode, std::string name) : Event<InputInfo>(InputInfo(key, mode, TETRA_INPUT_KEY_MODE), name) {};
	};
	class AnyKeyEvent : public Event<InputInfo>
	{
	public:
		int key;
		AnyKeyEvent(int key, int mode, std::string name) : Event<InputInfo>(InputInfo(mode, TETRA_INPUT_KEY_MODE), name), key(key) {};
	};


	class MouseEvent : public Event<InputInfo> {
	public:
		MouseEvent(int button, int mode, std::string name) : Event<InputInfo>(InputInfo(button, mode, TETRA_INPUT_MOUSE_BUTTON_MODE), name) {};
	};
	class AnyMouseEvent : public Event<InputInfo>
	{
	public:
		int key;
		AnyMouseEvent(int button, int mode, std::string name) : Event<InputInfo>(InputInfo(mode, TETRA_INPUT_MOUSE_BUTTON_MODE), name), key(button) {};
	};


	class MouseMoveEvent : public Event<InputInfo> {
	public:
		float deltaX, deltaY;
		float currentX, currentY;

		MouseMoveEvent(float deltaX, float deltaY, float currentX, float currentY, std::string name = "Mouse moved") :
			Event<InputInfo>(InputInfo(TETRA_INPUT_MOUSE_MOVE_MODE), name),
		deltaX(deltaX), deltaY(deltaY), currentX(currentX), currentY(currentY) {};
	};

	class InputManager
	{
	
	private:

		static std::map<int, std::string> keyNames;
		static std::map<int, std::string> mouseNames;
		void DispatchKeyEvent(InputInfo info);
		void DispatchMouseEvent(InputInfo info);
		void DispatchMouseMoveEvent(InputInfo info);

	public:

		std::string pressedKeys = "";

		bool anyPressed = false;
		bool keys[TETRA_INPUT_KEY_COUNT];
		bool mouseButtons[TETRA_INPUT_MOUSE_BUTTON_COUNT];

		float mousePosX, mousePosY;
		float deltaMouseX, deltaMouseY;

		EventDispatcher<InputInfo> keyDispatcher;

		InputManager();
		~InputManager();
		static std::string GetKeyName(int key);
		static InputManager* GetMain();

		void OnKeyDown(int key);
		void OnKeyUp(int key);
		void OnKeyPressed(int key);

		void OnMouseDown(int button);
		void OnMouseUp(int button);
		void OnMousePressed(int button);

		void OnMouseMove();

		bool IsKeyDown(int key) { return keys[key]; }
		bool IsMouseButtonDown(int button) { return mouseButtons[button]; }

		void Update();
		void UpdateKeys();
		void UpdateMouse();
		void UpdateMouseMovement(float newX, float newY);
	};

}

