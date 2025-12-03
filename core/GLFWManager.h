#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef TETRA_GLFW
#define TETRA_GLFW
#endif // !TETRA_GLFW

#ifndef TETRA_INPUT_FORMAT_DEFINED
#define TETRA_INPUT_FORMAT_DEFINED

#define TETRA_INPUT_KEY_COUNT 512
#define TETRA_INPUT_MOUSE_BUTTON_COUNT 8

#define TETRA_INPUT_CODE(info) info.bits.key
#define TETRA_INPUT_ACTION(info) info.bits.action
#define TETRA_INPUT_MODE(info) info.bits.mode
#define TETRA_INPUT_IS_ANY(info) info.bits.any

#define TETRA_INPUT_KEY_MODE 0
#define TETRA_INPUT_MOUSE_BUTTON_MODE 1
#define TETRA_INPUT_MOUSE_MOVE_MODE 2

#endif // !TETRA_INPUT_FORMAT_DEFINED

namespace TetraEngine
{
	class InputManager;

	class GLFWManager
	{
	private:

		static GLFWManager* current;
	    bool keys[TETRA_INPUT_KEY_COUNT];
	    bool prevKeys[TETRA_INPUT_KEY_COUNT];
	    bool mouseButtons[TETRA_INPUT_MOUSE_BUTTON_COUNT];
	    bool prevMouseButtons[TETRA_INPUT_MOUSE_BUTTON_COUNT];
		bool isFullscreen = false;

		void SetFullscreen();
		void SetWindowed();
	public:

		union KeyInfo
		{
		public:

			uint16_t raw;
			struct {
				uint16_t key : 9;
				uint16_t action : 2;
				uint16_t any : 1;
				uint16_t mode : 2;
				uint16_t _unused : 2;
			} bits;

			KeyInfo(int key, int action, int mode)
			{
				bits.key = key & 0b111111111;
				bits.action = action & 0b11;
				bits.any = 0;
				bits.mode = mode;
				bits._unused = 0;
			};
			KeyInfo(int action, int mode)
			{
				bits.key = 0;
				bits.action = action & 0b11;
				bits.any = 1;
				bits.mode = mode;
				bits._unused = 0;
			};
			KeyInfo(int mode)
			{
				bits.key = 0;
				bits.action = 0;
				bits.any = 0;
				bits.mode = mode;
				bits._unused = 0;
			};
			bool operator <(const KeyInfo& other) const { return raw < other.raw; }
			bool operator ==(const KeyInfo& other) const { return raw == other.raw; }
			operator int() const { return raw; }
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

