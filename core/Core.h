#pragma once

#include "tetrapc.h"

#include "Application.h"
#include "ConsoleManager.h"
#include "InputManager.h"
#include "ImGuiManager.h"
#include "GLFWManager.h"
#include "rendering/Viewport.h"
#include "ecs/ECS.h"

namespace TetraEngine {

	class Core
	{
	private:
		Core() = default;
		~Core() = default;

	public:
		Core(const Core&) = delete;
		Core& operator=(const Core&) = delete;

		static uint appWidth, appHeight;
		static float lastMouseX, lastMouseY;
		static bool cursorEnabled;

		static Application* application;
		static GLFWManager* glfwManager;
		static ImGuiManager* imguiManager;
		static InputManager* inputManager;
		static Viewport* mainViewport;

		template <typename T>
		static void CreateApplication() {
			if (std::is_base_of<Application, T>::value)
				application = new T();
			else
				throw std::invalid_argument("input is not an application type");
		}

		static int Initialize();
		static void InitializePresets();
		static void Update();
		static void UpdateOverlay();
		static void CleanUp();
		static void AfterUpdate();
		static void CleanUpPresets();
		static ECS::ECS& GetMainECS();

		//console
		static void processConsole();
		static void CloseApplication(const Event<InputInfo>& ev);
		//input
		static void processInput(GLFWwindow* window);
	};
}
