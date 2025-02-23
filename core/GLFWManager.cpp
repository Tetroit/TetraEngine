#include "tetrapc.h"
#include "GLFWManager.h"
#include "rendering/Scene.h"
#include "rendering/Camera.h"
#include "InputManager.h"
#include "Core.h"

using namespace TetraEngine;

GLFWManager* GLFWManager::current = nullptr;

GLFWManager::GLFWManager(int width, int height) : width(width), height(height)
{
	//window

	glfwInit();
	window = glfwCreateWindow(width, height, "showcase", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);

	ToggleCursor(cursorEnabled);

	glfwMakeContextCurrent(window);

	if (current == nullptr)
		current = this;
}
GLFWManager::~GLFWManager()
{
	glfwTerminate();
}
GLFWManager* GLFWManager::get()
{
	return Core::glfwManager;
}
void GLFWManager::mouse_callback(::GLFWwindow* window, double xposIn, double yposIn) {

	if (current->sendMouseMoveEvents)
	{
		current->inputManager->UpdateMouseMovement(xposIn, yposIn);
	}
}

void GLFWManager::mouse_button_callback(::GLFWwindow* window, int button, int action, int mods)
{
	if (current->sendMouseClickEvents)
	{
		if (action == GLFW_PRESS)
			current->inputManager->OnMouseDown(button);
		if (action == GLFW_RELEASE)
			current->inputManager->OnMouseUp(button);
	}
}

void GLFWManager::key_callback(::GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (current->sendKeyboardEvents)
	{
		if (action == GLFW_PRESS)
			current->inputManager->OnKeyDown(key);
		if (action == GLFW_RELEASE)
			current->inputManager->OnKeyUp(key);
	}
}

void GLFWManager::ToggleCursor(bool toOn)
{
	cursorEnabled = toOn;
	if (toOn)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GLFWManager::ToggleKeyboardEvents(bool toOn)
{
	sendKeyboardEvents = toOn;
	if (!toOn)
	{
		for (int i = 0; i < TETRA_INPUT_KEY_COUNT; i++)
			inputManager->keys[i] = false;
	}
}

void GLFWManager::ToggleMouseClickEvents(bool toOn)
{
	sendMouseClickEvents = toOn;
	if (!toOn)
	{
		for (int i = 0; i < TETRA_INPUT_MOUSE_BUTTON_COUNT; i++)
			inputManager->mouseButtons[i] = false;
	}
}

void GLFWManager::ToggleMouseMoveEvents(bool toOn)
{
	sendMouseMoveEvents = toOn;
}
