#pragma once
#include <array>

#include "utils/Event.h"
#include "GLFWManager.h"

namespace TetraEngine
{
	#ifdef TETRA_GLFW
	typedef GLFWManager::InputEvent InputEvent;
	typedef GLFWManager::InputEvent::InputEventType InputEventType;
	#endif // TETRA_GLFW

	#ifndef TETRA_INPUT_FORMAT_DEFINED
	#error No input format found
	#endif // !TETRA_INPUT_FORMAT_DEFINED

	class KeyEvent : public Event<InputEvent> {
	public:
		KeyEvent(int key, int mode) : Event(
			InputEvent(InputEventType::KEY, mode, key),
				"KeyEvent"
			) {};
		constexpr static InputEvent GetLink(int mode, int key) {
			return InputEvent(InputEventType::KEY, mode, key);
		}
	};
	class AnyKeyEvent : public Event<InputEvent>
	{
	public:
		int key;
		AnyKeyEvent(int key, int mode) : Event(
			InputEvent(
				InputEventType::KEY_ANY, mode),
				"AnyKeyEvent"
			), key(key) {}
		constexpr static InputEvent GetLink(int mode) {
			return InputEvent(InputEventType::KEY_ANY, mode);
		}
	};
	class MouseEvent : public Event<InputEvent> {
	public:
		MouseEvent(int key, int mode) : Event(
			InputEvent(
				InputEventType::MB, mode, key),
				"MouseEvent"
			) {};
		constexpr static InputEvent GetLink(int mode, int key) {
			return InputEvent(InputEventType::MB, mode, key);
		}
	};
	class AnyMouseEvent : public Event<InputEvent>
	{
	public:
		int key;
		AnyMouseEvent(int key, int mode) : Event(
			InputEvent(
				InputEventType::MB_ANY, mode),
				"AnyMouseEvent"
			), key(key) {}
		constexpr static InputEvent GetLink(int mode) {
			return InputEvent(InputEventType::MB_ANY, mode);
		}
	};


	class MouseMoveEvent : public Event<InputEvent> {
	public:
		float deltaX, deltaY;
		float currentX, currentY;

		MouseMoveEvent(float deltaX, float deltaY, float currentX, float currentY, std::string name = "Mouse moved") :
			Event(InputEvent(InputEventType::MOUSE_MOVE), name),
		deltaX(deltaX), deltaY(deltaY), currentX(currentX), currentY(currentY) {};
		constexpr static InputEvent GetLink() {
			return InputEvent(InputEventType::MOUSE_MOVE);
		}
	};

	class InputManager
	{
		typedef std::array<std::pair<int, std::string_view>, GLFWManager::key_count> keyArray;
		typedef std::array<std::pair<int, std::string_view>, GLFWManager::mb_count> mbArray;
	private:
		static const keyArray keyNames;
		static const mbArray mouseNames;
		void DispatchKeyEvent(int key, int mode);
		void DispatchMouseEvent(int key, int mode);
		void DispatchMouseMoveEvent();

		EventDispatcher<InputEvent> gameDispatcher;
		EventDispatcher<InputEvent> editorDispatcher;

	public:

		std::string pressedKeys = "";

		bool anyPressed = false;
		bool keys[GLFWManager::key_count];
		bool mouseButtons[GLFWManager::mb_count];

		float mousePosX, mousePosY;
		float deltaMouseX, deltaMouseY;

		InputManager();
		~InputManager();

		static constexpr std::string_view GetKeyName(int key);
		static constexpr std::string_view GetMouseButtonName(int key);
		static InputManager* GetMain();

		void OnKeyDown(int key);
		void OnKeyUp(int key);
		void OnKeyPressed(int key);

		void OnMouseDown(int button);
		void OnMouseUp(int button);
		void OnMousePressed(int button);

		void OnMouseMove();

		bool IsKeyDown(int key) const;
		bool IsMouseButtonDown(int button) const;
		bool IsKeyDownEditor(int key) const;
		bool IsMouseButtonDownEditor(int button) const;

		void Update();
		void UpdateKeys();
		void UpdateMouse();
		void UpdateMouseMovement(float newX, float newY);

		EventDispatcher<InputEvent>& GetGameDispatcher();
		EventDispatcher<InputEvent>& GetEditorDispatcher();
	};

}

