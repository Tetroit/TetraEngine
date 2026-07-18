#pragma once

#include "tetrapc.h"

#include "Application.h"
#include "Editor.h"
#include "GLFWManager.h"
#include "GlobalContext.h"


#define TETRA_USE_MAIN_ECS auto* ecs = TetraEngine::Core::GetMainECS();
#define TETRA_USE_MAIN_PHYSICS_INSTANCE auto* physicsInstance = TetraEngine::Core::GetPhysicsInstance();
#define TETRA_USE_MAIN_PHYSICS auto* physics = TetraEngine::Core::GetPhysics();
#define TETRA_USE_MAIN_PHYSICS_SCENE auto* physicsScene = TetraEngine::Core::GetPhysicsScene();
#define TETRA_USE_GAME_INPUT auto* input = TetraEngine::Core::GetGameDispatcher();
#define TETRA_USE_EDITOR_INPUT auto* input = TetraEngine::Core::GetEditorDispatcher();

namespace TetraEngine {
}

namespace TetraEngine {

	class TypeRegistry;
	class SharedContentRegistry;
	class UIManager;
	class InputManager;
	class DebugManager;
	class Editor;
	namespace ECS {
		class ECS;
	}

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
		static inline GlobalContext globalContext;

		template <typename T>
		static void CreateApplication() {
			if (std::is_base_of<Application, T>::value)
				globalContext.application = new T();
			else
				throw std::invalid_argument("input is not an application type");
		}

		static int Initialize();
		static void InitializeTypes();
		static void InitializePresets();
		static void BindEvents();
		static void Update();
		static void UpdateOverlay();
		static void CleanUp();
		static void AfterUpdate();
		static void CleanUpPresets();

		static ECS::ECS* GetMainECS(){return globalContext.ecs;}
		static InputManager* GetInputManager() {return globalContext.inputManager;}
		static Editor::Mode GetEditorMode() {return globalContext.editor->GetMode();}
		static bool IsFocusedOnViewport() {return globalContext.editor->IsFocused();}
		static SharedContentRegistry* GetResourceManager() {return globalContext.resources;}
		static Viewport* GetViewport() {return globalContext.mainViewport;}
		static TypeRegistry* GetTypeRegistry() {return globalContext.typeRegistry;}
		static GLFWManager * GetGLFWManager() {return globalContext.glfwManager;}
		static DestroyManager* GetDestroyManager(){return globalContext.destroyManager;}
		static Application* GetApplication(){return globalContext.application;}

		static EventDispatcher<InputEvent>* GetGameDispatcher(){return &globalContext.inputManager->GetGameDispatcher();};
		static EventDispatcher<InputEvent>* GetEditorDispatcher(){return &globalContext.inputManager->GetEditorDispatcher();}
		//console
		static void processConsole();
		//input
		static void processInput(GLFWwindow* window);
		static void close();
	};
}
