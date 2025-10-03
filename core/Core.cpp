#include <tetrapc.h>
#include "Core.h"

#include "MyApplication.h"
#include "utils/Event.h"
#include "utils/Time.h"
#include "rendering/Scene.h"
#include "FreeType.h"
#include "DestroyManager.h"
#include "rendering/PointLight.h"
#include "rendering/Material.h"
#include "rendering/LightRenderer.h"
#include "rendering/VertexData.h"
#include "rendering/Camera.h"
#include "rendering/Texture2D.h"
#include "rendering/Skybox.h"
#include "utils/OBJParser.h"

#ifdef NDEBUG
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformanc = 1;
}
#endif //NDEBUG

using namespace TetraEngine;

unsigned int Core::appWidth = 1920;
unsigned int Core::appHeight = 1080;

float Core::lastMouseX;
float Core::lastMouseY;

bool Core::cursorEnabled = false;

Application* Core::application = nullptr;
GLFWManager* Core::glfwManager = nullptr;
ImGuiManager* Core::imguiManager = nullptr;
InputManager* Core::inputManager = nullptr;
Viewport* Core::mainViewport = nullptr;

ECS::ECS & Core::GetMainECS() {
	static ECS::ECS mainEcs;
	return mainEcs;
}

void Core::processConsole() {

	std::string command;
	while (!glfwWindowShouldClose(glfwManager->window))
	{
		std::getline(std::cin, command);
		//ConsoleManager::ParseCommand(command);
	}
}

void Core::CloseApplication(const Event<InputInfo>& ev) {

	if (ev.GetType().bits.any) return;
	KeyEvent keyEvent = ev.ToType<KeyEvent>();

	if (ev.GetType().bits.key == GLFW_KEY_ESCAPE 
	&& ev.GetType().bits.action == GLFW_PRESS)
		glfwSetWindowShouldClose(glfwManager->window, true);
}
void Core::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (imguiManager->allowSceneInteraction) {

		if (inputManager->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
			glfwManager->ToggleCursor(false);
		else
		{
			imguiManager->allowSceneInteraction = false;
			glfwManager->ToggleCursor(true);
		}

		inputManager->Update();
	}

}

int Core::Initialize()
{
	std::srand(time(NULL));

	inputManager = new InputManager();

	glfwManager = new GLFWManager(1920, 1080);
	glfwManager->inputManager = inputManager;

	//window

	//ConsoleManager::Initialize(glfwManager->window);

	//opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n';
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n\n";

	glEnable(GL_DEPTH_TEST);

	//imgui
	imguiManager = new ImGuiManager();

	Camera* mainCamera = new Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	mainCamera->SetProjection((float)glm::radians(45.0), (float)Core::appWidth / (float)Core::appHeight, 0.1f, 100.0f);
	Camera::SetMain(mainCamera);

	mainViewport = new Viewport(1280, 720, *mainCamera);
	//presets
	Core::InitializePresets();

	return 0;
}

void Core::InitializePresets()
{
	if (FreeType::Initialise()) std::cout << "freetype initialised successfully\n";
	Shader::InitialiseShaders();
	std::cout << "shaders initialized\n";
	VertexData::InitialisePrefabs();
	std::cout << "models initialized\n";
	MeshRenderer::InitialiseRenderer();
	//LightRenderer::InitialiseRenderer();
	std::cout << "renderers initialized\n";
	Material::Initialize();
	std::cout << "materials initialized\n";
}


void Core::Update()
{

	Time::Update();
	Core::processInput(Core::glfwManager->window);

	imguiManager->Update();

	Scene::currentScene->Update();

	if (glfwGetWindowAttrib(Core::glfwManager->window, GLFW_ICONIFIED) != 0)
	{
		ImGui_ImplGlfw_Sleep(10);
	}

	int display_w, display_h;
	glfwGetFramebufferSize(Core::glfwManager->window, &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	application->Update();

	mainViewport->Bind();
	Scene::currentScene->Render(Camera::main);

	imguiManager->testTex = mainViewport->GetTexture();


}
void Core::UpdateOverlay()
{
	//transparent render

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//overlay

	glm::mat4 proj = glm::ortho(0.0f, (float)mainViewport->GetWidth(), 0.0f, (float)mainViewport->GetHeight());
	Shader::textShader->Use();
	Shader::textShader->SetMat4("projection", proj);

	FreeType::RenderText("Keys pressed: " + inputManager->pressedKeys, 10, 20, 1, glm::vec3(1, 1, 1));

	mainViewport->Unbind(appWidth, appHeight);
	imguiManager->Render();
}
void Core::AfterUpdate()
{
	glfwSwapBuffers(glfwManager->window);
	glfwPollEvents();

	DestroyManager::get()->deleteAll();
}
void Core::CleanUp() {
	delete mainViewport;
	delete application;
	delete imguiManager;
	delete glfwManager;
	delete inputManager;
}
