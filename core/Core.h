#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>
#include <thread>

#include "Application.h"
#include "ConsoleManager.h"
#include "InputManager.h"
#include "ImGuiManager.h"
#include "GLFWManager.h"
#include "rendering/Viewport.h"

namespace TetraEngine {

	class Core
	{
	private:
		Core() = default;
	public:

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

	//console
	static void processConsole();
	static void CloseApplication(const Event<InputInfo>& ev);
	//input
	static void processInput(GLFWwindow* window);
	};
}
