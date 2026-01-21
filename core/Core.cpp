#include "tetrapc.h"
#include "Core.h"

#include <backends/imgui_impl_glfw.h>

#include "applications/MyApplication.h"
#include "../applications/PhysicsTest.h"


#include "utils/Event.h"
#include "utils/Time.h"
#include "rendering/Scene.h"
#include "FreeType.h"
#include "DestroyManager.h"
#include "Editor.h"
#include "assimp/Logger.hpp"
#include "physics/RigidBody.h"
#include "rendering/Camera.h"
#include "rendering/Material.h"
#include "rendering/LightRenderer.h"
#include "rendering/VertexData.h"
#include "rendering/ViewportCamera.h"
#include "rendering/Viewport.h"
#include "utils/OBJParser.h"

//#ifdef NDEBUG

	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//#endif //NDEBUG

using namespace TetraEngine;

float Core::lastMouseX;
float Core::lastMouseY;

bool Core::cursorEnabled = false;

Application* Core::application = nullptr;
GLFWManager* Core::glfwManager = nullptr;
ImGuiManager* Core::imguiManager = nullptr;
InputManager* Core::inputManager = nullptr;
Viewport* Core::mainViewport = nullptr;
PhysXInstance* Core::physxInstance = nullptr;
DestroyManager* Core::destroyManager = nullptr;
Editor* Core::editor = nullptr;

ECS::ECS & Core::GetMainECS() {
	static ECS::ECS mainEcs;
	return mainEcs;
}

PxPhysics * Core::GetPhysics() {
	return physxInstance->GetPhysics();
}

PhysXInstance * Core::GetPhysicsInstance() {
	return physxInstance;
}

PhysicsScene* Core::GetPhysicsScene() {
	return physxInstance->GetActiveScene();
}

InputManager* Core::GetInputManager() {
	return inputManager;
}

Editor::Mode Core::GetEditorMode() {
	return editor->GetMode();
}

bool Core::IsFocusedOnViewport() {
	return editor->IsFocused();
}

EventDispatcher<InputEvent> * Core::GetGameDispatcher() {
    return &inputManager->GetGameDispatcher();
}
EventDispatcher<InputEvent> * Core::GetEditorDispatcher() {
	return &inputManager->GetEditorDispatcher();
}

void Core::processConsole() {

    std::string command;
    while (!glfwWindowShouldClose(glfwManager->window))
    {
        std::getline(std::cin, command);
        //ConsoleManager::ParseCommand(command);
    }
}

void Core::processInput(GLFWwindow* window)
{
    if (glfwManager->WasPressedThisFrameKey(GLFW_KEY_M)) {
    	if (!imguiManager->IsMaximized() && Scene::currentScene->gameCamera == nullptr) {
    		LOG_ERR_FROM("Core::processInput()", "No game camera");
    	}
    	else {
    		switch (GetEditorMode()) {
    			case Editor::PLAY:
    				editor->SwitchMode(Editor::EDIT);
    				break;
    			case Editor::EDIT:
    				editor->SwitchMode(Editor::PLAY);
    				break;
    		}
    	}
    }
	inputManager->Update();
}

void Core::close() {
    glfwSetWindowShouldClose(glfwManager->window, true);
}

int Core::Initialize()
{
	std::srand(time(nullptr));

	inputManager = new InputManager();
	inputManager->GetGameDispatcher().Disable();

    destroyManager = new DestroyManager();

	glfwManager = new GLFWManager(1280, 720);
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

	//physx
	physxInstance = new PhysXInstance();
	physxInstance->Initialise();

	//imgui
	imguiManager = new ImGuiManager();
	mainViewport = new Viewport(glfwManager->width, glfwManager->height,  nullptr);

	editor = new Editor(inputManager,imguiManager,glfwManager);
	//presets
	InitializePresets();
	BindEvents();

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
	std::cout << "renderers initialized\n";
	Material::Initialize();
	std::cout << "materials initialized\n";
}

void Core::BindEvents() {
	TETRA_USE_MAIN_ECS
	auto& transformAddedEv = ecs.OnComponentCreated<Transform>();
	transformAddedEv.AddCallback(Transform::ComponentCreate, "Transform");
	auto& rigidBodyAddedEv = ecs.OnComponentCreated<RigidBody>();
	rigidBodyAddedEv.AddCallback(RigidBody::ComponentCreate, "RigidBody");
	auto& cameraAddedEv = ecs.OnComponentCreated<Camera>();
	cameraAddedEv.AddCallback(Camera::ComponentCreate, "Camera");

	imguiManager->BindEvents();
}


void Core::Update() {
	Time::Update();

    glfwPollEvents();
	processInput(glfwManager->window);

	Scene::currentScene->Update();
	application->Update();

	if (glfwGetWindowAttrib(glfwManager->window, GLFW_ICONIFIED) != 0)
	{
		ImGui_ImplGlfw_Sleep(10);
	}

	int display_w, display_h;
	glfwGetFramebufferSize(glfwManager->window, &display_w, &display_h);

	//glViewport(0, 0, display_w, display_h);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	imguiManager->StartRender();

#if TETRA_DEBUG_UI
	imguiManager->RenderApp();
	editor->Update();
	mainViewport->Bind();
#endif

	Scene::currentScene->Render();
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

	//FreeType::RenderText("Keys pressed: " + inputManager->pressedKeys, 10, 20, 1, glm::vec3(1, 1, 1));

#if TETRA_DEBUG_UI

	mainViewport->Unbind(glfwManager->width, glfwManager->height);
#endif

	imguiManager->EndRender();

}
void Core::AfterUpdate()
{
	glfwSwapBuffers(glfwManager->window);

	application->LateUpdate();
    destroyManager->Update();
    glfwManager->Update();
}
void Core::CleanUp() {
	delete mainViewport;
	delete application;
	delete editor;
	delete imguiManager;
	delete glfwManager;
	delete inputManager;
	delete physxInstance;
    delete destroyManager;
}
