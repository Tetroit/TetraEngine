#pragma once

#include "tetrapc.h"

#include "Application.h"
#include "ConsoleManager.h"
#include "DestroyManager.h"
#include "InputManager.h"
#include "UI/ImGuiManager.h"
#include "GLFWManager.h"
#include "rendering/Viewport.h"
#include "ecs/ECS.h"
#include "physics/PhysX.h"


#define TETRA_USE_MAIN_ECS auto& ecs = TetraEngine::Core::GetMainECS();
#define TETRA_USE_MAIN_PHYSICS_INSTANCE auto* physicsInstance = TetraEngine::Core::GetPhysicsInstance();
#define TETRA_USE_MAIN_PHYSICS auto* physics = TetraEngine::Core::GetPhysics();
#define TETRA_USE_MAIN_PHYSICS_SCENE auto* physicsScene = TetraEngine::Core::GetPhysicsScene();
#define TETRA_USE_MAIN_INPUT auto* input = TetraEngine::Core::GetInputDispatcher();

namespace TetraEngine {

	class Core
	{
	private:
		Core() = default;
		~Core() = default;

	public:
		Core(const Core&) = delete;
		Core& operator=(const Core&) = delete;

		static float lastMouseX, lastMouseY;
		static bool cursorEnabled;

		static Application* application;
		static GLFWManager* glfwManager;
		static ImGuiManager* imguiManager;
		static InputManager* inputManager;
		static Viewport* mainViewport;
		static PhysXInstance* physxInstance;
	    static DestroyManager* destroyManager;

		template <typename T>
		static void CreateApplication() {
			if (std::is_base_of<Application, T>::value)
				application = new T();
			else
				throw std::invalid_argument("input is not an application type");
		}

		static int Initialize();
		static void InitializePresets();
		static void BindEvents();
		static void Update();
		static void UpdateOverlay();
		static void CleanUp();
		static void AfterUpdate();
		static void CleanUpPresets();
		static ECS::ECS& GetMainECS();
		static physx::PxPhysics* GetPhysics();
		static PhysXInstance* GetPhysicsInstance();
		static PhysicsScene* GetPhysicsScene();
		static EventDispatcher<InputInfo>* GetInputDispatcher();
		//console
		static void processConsole();
		static void CloseApplication(const Event<InputInfo>& ev);
		//input
		static void processInput(GLFWwindow* window);
	};
}
