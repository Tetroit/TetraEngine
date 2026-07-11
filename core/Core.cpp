#include "tetrapc.h"
#include "Core.h"

#include <backends/imgui_impl_glfw.h>

#include "applications/MyApplication.h"
#include "../applications/PhysicsTest.h"


#include "utils/Event.h"
#include "utils/Time.h"
#include "utils/Types.h"
#include "rendering/Scene.h"
#include "FreeType.h"
#include "DestroyManager.h"
#include "Editor.h"
#include "assimp/Logger.hpp"
#include "rendering/Camera.h"
#include "rendering/Material.h"
#include "rendering/LightRenderer.h"
#include "rendering/VertexData.h"
#include "rendering/ViewportCamera.h"
#include "rendering/Viewport.h"
#include "resources/SharedContentRegistry.h"
#include "utils/OBJParser.h"

//#ifdef NDEBUG
// extern "C"{
// 	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
// 	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
// }

using namespace TetraEngine;

float Core::lastMouseX;
float Core::lastMouseY;

bool Core::cursorEnabled = false;

void Core::processConsole() {

    std::string command;
    while (!glfwWindowShouldClose(globalContext.glfwManager->window))
    {
        std::getline(std::cin, command);
        //ConsoleManager::ParseCommand(command);
    }
}

void Core::processInput(GLFWwindow* window)
{
    if (globalContext.glfwManager->WasPressedThisFrameKey(GLFW_KEY_M)) {
    	if (!globalContext.imguiManager->IsMaximized() && Scene::currentScene->gameCamera == nullptr) {
    		LOG_ERR_FROM("Core::processInput()", "No game camera");
    	}
    	else {
    		switch (GetEditorMode()) {
    			case Editor::PLAY:
    				globalContext.editor->SwitchMode(Editor::EDIT);
    				break;
    			case Editor::EDIT:
    				globalContext.editor->SwitchMode(Editor::PLAY);
    				break;
    		}
    	}
    }
	globalContext.inputManager->Update();
}

void Core::close() {
    glfwSetWindowShouldClose(globalContext.glfwManager->window, true);
}

int Core::Initialize()
{
	std::srand(time(nullptr));

	InitializeTypes();
	globalContext.resources = new SharedContentRegistry();
	globalContext.ecs = new ECS::ECS();

	globalContext.inputManager = new InputManager();
	globalContext.inputManager->GetGameDispatcher().Disable();

    globalContext.destroyManager = new DestroyManager();

	//opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	//window
	globalContext.glfwManager = new GLFWManager(1280, 720);
	globalContext.glfwManager->inputManager = globalContext.inputManager;


	//ConsoleManager::Initialize(glfwManager->window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << '\n';
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << '\n';
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";
	std::cout << "Working directory: " <<std::filesystem::current_path() << "\n\n";


	glEnable(GL_DEPTH_TEST);

	//imgui
	globalContext.imguiManager = new ImGuiManager();
	globalContext.mainViewport = new Viewport(
		globalContext.glfwManager->width,
		globalContext.glfwManager->height,
		nullptr);

	globalContext.editor = new Editor(
		globalContext.inputManager,
		globalContext.imguiManager,
		globalContext.glfwManager);
	//presets
	InitializePresets();
	BindEvents();

	return 0;
}

void Core::InitializeTypes() {
	globalContext.typeRegistry = new TypeRegistry();
	globalContext.typeRegistry->Register<Texture2D>();
	globalContext.typeRegistry->Register<Material>();
	globalContext.typeRegistry->Register<Shader>();
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
	auto& transformAddedEv = ecs->OnComponentCreated<Transform>();
	transformAddedEv.AddCallback(Transform::ComponentCreate, "Transform");
	auto& cameraAddedEv = ecs->OnComponentCreated<Camera>();
	cameraAddedEv.AddCallback(Camera::ComponentCreate, "Camera");

	globalContext.imguiManager->BindEvents();
}


void Core::Update() {
	Time::Update();

    glfwPollEvents();
	processInput(globalContext.glfwManager->window);

	Scene::currentScene->Update();
	globalContext.application->Update();

	if (glfwGetWindowAttrib(globalContext.glfwManager->window, GLFW_ICONIFIED) != 0)
	{
		ImGui_ImplGlfw_Sleep(10);
	}

	int display_w, display_h;
	glfwGetFramebufferSize(globalContext.glfwManager->window, &display_w, &display_h);

	//glViewport(0, 0, display_w, display_h);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	globalContext.imguiManager->StartRender();

#if TETRA_DEBUG_UI
	globalContext.imguiManager->RenderApp();
	globalContext.editor->Update();
	globalContext.mainViewport->Bind();
#endif

	Scene::currentScene->Render();
}
void Core::UpdateOverlay()
{
	//transparent render

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//overlay

	glm::mat4 proj = glm::ortho(0.0f, (float)globalContext.mainViewport->GetWidth(), 0.0f, (float)globalContext.mainViewport->GetHeight());
	Shader::textShader->Use();
	Shader::textShader->SetMat4("projection", proj);

	//FreeType::RenderText("Keys pressed: " + inputManager->pressedKeys, 10, 20, 1, glm::vec3(1, 1, 1));

#if TETRA_DEBUG_UI

	globalContext.mainViewport->Unbind(globalContext.glfwManager->width, globalContext.glfwManager->height);
#endif

	globalContext.imguiManager->EndRender();

}
void Core::AfterUpdate()
{
	glfwSwapBuffers(globalContext.glfwManager->window);

	globalContext.application->LateUpdate();
    globalContext.destroyManager->Update();
    globalContext.glfwManager->Update();
}
void Core::CleanUp() {
	delete globalContext.mainViewport;
	delete globalContext.application;
	delete globalContext.editor;
	delete globalContext.imguiManager;
	delete globalContext.glfwManager;
	delete globalContext.inputManager;
    delete globalContext.destroyManager;
}
