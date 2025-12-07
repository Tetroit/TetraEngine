#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef TETRA_GLFW
#define TETRA_GLFW
#endif // !TETRA_GLFW

#ifndef TETRA_INPUT_FORMAT_DEFINED
#define TETRA_INPUT_FORMAT_DEFINED
#endif // !TETRA_INPUT_FORMAT_DEFINED

namespace TetraEngine
{
	class InputManager;

	class GLFWManager
	{
	public:
		static constexpr int key_count = 512;
		static constexpr int mb_count = 8;
	private:
		static GLFWManager* current;
	    bool keys[key_count];
	    bool prevKeys[key_count];
	    bool mouseButtons[mb_count];
	    bool prevMouseButtons[mb_count];
		bool isFullscreen = false;

		void SetFullscreen();
		void SetWindowed();
	public:

		struct
		InputEvent
		{
			enum InputEventType {
				KEY = 0,
				KEY_ANY = 1,
				MB = 2,
				MB_ANY = 3,
				MOUSE_MOVE = 4
			};

			InputEventType type;
			int key;
			int mode;
			constexpr InputEvent(InputEventType type, int mode = -1, int key = -1) : type(type), key(key), mode(mode) {}
			bool operator == (const InputEvent& other) const {
				return type == other.type && key == other.key && mode == other.mode;
			}
		};

		bool sendMouseMoveEvents = true;
		bool sendKeyboardEvents = true;
		bool sendMouseClickEvents = true;
	
		GLFWwindow* window = nullptr;
		InputManager* inputManager = nullptr;

		bool cursorEnabled = true;
		unsigned int width = 1920, height = 1080;
		int widthWindowed = 1920, heightWindowed = 1080;
		int xWindowed = 0, yWindowed = 0;
		float lastMouseX = 0, lastMouseY = 0;

		GLFWManager(int width, int height);

		~GLFWManager();
		static GLFWManager* get();
		static void DisplayModes();
		void SetScreenMode (bool fullscreen);
		[[nodiscard]] bool IsFullscreen() const;

		static void mouse_callback(::GLFWwindow* window, double xposIn, double yposIn);

		static void mouse_button_callback(::GLFWwindow* window, int button, int action, int mods);

		static void key_callback(::GLFWwindow* window, int key, int scancode, int action, int mods);

		static void framebuffer_size_callback(::GLFWwindow* window, int width, int height);

		void ToggleCursor(bool toOn);
		void ToggleKeyboardEvents(bool toOn);
		void ToggleMouseClickEvents(bool toOn);
		void ToggleMouseMoveEvents(bool toOn);

	    bool WasPressedThisFrameKey(int key);
	    bool WasPressedThisFrameMouse(int button);
	    bool WasReleasedThisFrameKey(int key);
	    bool WasReleasedThisFrameMouse(int button);

	    void Update();

	};

}


namespace std {
	template<>
	struct hash<TetraEngine::GLFWManager::InputEvent> {
		std::size_t operator()(const TetraEngine::GLFWManager::InputEvent& e) const noexcept {
			std::size_t h1 = std::hash<int>{}(e.type);
			std::size_t h2 = std::hash<int>{}(e.mode);
			std::size_t h3 = std::hash<int>{}(e.key);

			std::size_t seed = h1;
			seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}