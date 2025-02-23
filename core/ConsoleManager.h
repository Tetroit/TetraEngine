#pragma once

#include "tetrapc.h"

namespace TetraEngine {
	class GameObject;
	struct ::GLFWwindow;

	static class ConsoleManager {


	private:

		static enum Command
		{
			UNKNOWN,
			G_Quit,
			G_FPS,
			G_Tree,
			G_GameObject,
			GO_Transform,
			GO_Parent,
			GO_Destroy
		};
		static enum Type
		{
			T_Null,
			T_GameObject
		};

		static void* context;
		static Type contextType;

		static void WriteObject(GameObject* obj, std::string prefix);
		static ConsoleManager::Command IdentifyCommand(std::string str);

	public:

		static HANDLE console;
		static ::GLFWwindow* window;

		static void Initialize(GLFWwindow* window);
		template <typename mat>
		static void PrintMatrix(mat matrix);
		static void ParseCommand(std::string command);
	};
}