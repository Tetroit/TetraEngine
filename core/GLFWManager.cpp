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

	if (glfwInit()) {
		std::cout << "GLFW initialized" << std::endl;
	}
	else {
		std::cout << "Failed to initialize GLFW" << std::endl;
		throw std::runtime_error("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_REFRESH_RATE, 30);

	//DisplayModes();

	//glfwCreateWindow -> glfwCreateWindow -> _glfwCrerateWindowWin32 -> _glfwInitWGL can cause issue with duplicate monitor

	window = glfwCreateWindow(width, height, "showcase", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
#if !TETRA_DEBUG_UI
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
#endif

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

void GLFWManager::DisplayModes() {
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);

	for (int i = 0; i < count; ++i) {
		const char* name = glfwGetMonitorName(monitors[i]);
		std::cout << "Monitor: " << i << " " << name << '\n';

		int xpos, ypos;
		glfwGetMonitorPos(monitors[i], &xpos, &ypos);
		std::cout << "Position: " << xpos << " " << ypos << '\n';

		const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
		std::cout << " Current: x:" << mode->width << " y:" << mode->height <<
		" refresh rate:" << mode->refreshRate <<
		" color bits:" << mode->redBits << mode->greenBits << mode->blueBits << '\n';

		int modeCount;
		const GLFWvidmode* modes = glfwGetVideoModes(monitors[i], &modeCount);
		std::cout << "  Supported modes:\n";
		for (int j = 0; j < modeCount; ++j) {
			std::cout << "  " << modes[j].width << " x " << modes[j].height;
			std::cout << " refresh rate: " << modes[j].refreshRate;
			std::cout << " color bits: " << modes[j].redBits << modes[j].greenBits << modes[j].blueBits << '\n';
		}
	}
	// DISPLAY_DEVICE dd = { sizeof(dd) };
	// EnumDisplayDevices(NULL, 0, &dd, 0); // primary monitor
	// HDC dc = CreateDCW(L"DISPLAY", dd.DeviceName, NULL, NULL); // real DC
	//
	// PIXELFORMATDESCRIPTOR pfd;
	// pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	// pfd.iPixelType = PFD_TYPE_RGBA;
	// pfd.cColorBits = 32;
	// pfd.cDepthBits = 24;
	// pfd.cStencilBits = 8;
	// pfd.iLayerType = PFD_MAIN_PLANE;
	//
	// ChoosePixelFormat(dc, &pfd);
	//
	// HWND hwnd = CreateWindowExA(0, "STATIC", "dummy", WS_OVERLAPPEDWINDOW,
	// 							CW_USEDEFAULT, CW_USEDEFAULT, 100, 100,
	// 							NULL, NULL, GetModuleHandle(NULL), NULL);
	//
	// HDC hdc = GetDC(hwnd);
	// count = DescribePixelFormat(hdc, 1, sizeof(pfd), &pfd);
	//
	// for (int i = 1; i <= count; ++i) {
	// 	DescribePixelFormat(hdc, i, sizeof(pfd), &pfd);
	// 	printf("PixelFormat #%d: color %d bits, depth %d, stencil %d\n",
	// 		i, pfd.cColorBits, pfd.cDepthBits, pfd.cStencilBits);
	// }
	// ReleaseDC(hwnd, hdc);
	// DestroyWindow(hwnd);
}

void GLFWManager::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

	if (current->sendMouseMoveEvents)
	{
		current->inputManager->UpdateMouseMovement(static_cast<float>(xposIn), static_cast<float>(yposIn));
	}
}

void GLFWManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (current->sendMouseClickEvents)
	{
		if (action == GLFW_PRESS)
			current->inputManager->OnMouseDown(button);
		if (action == GLFW_RELEASE)
			current->inputManager->OnMouseUp(button);
	}
}

void GLFWManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (current->sendKeyboardEvents)
	{
		if (action == GLFW_PRESS)
			current->inputManager->OnKeyDown(key);
		if (action == GLFW_RELEASE)
			current->inputManager->OnKeyUp(key);
	}
}

void GLFWManager::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	Camera::main->SetProjection(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
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
