#include "tetrapc.h"
#include "ConsoleManager.h"

#include <glm/glm.hpp>
// #include <glm/gtx/string_cast.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "utils/Utils.h"
#include "utils/Time.h"
#include "rendering/Scene.h"
#include "DestroyManager.h"
#include "GameObject.h"

using namespace TetraEngine;

GLFWwindow* ConsoleManager::window = nullptr;
HANDLE ConsoleManager::console = GetStdHandle(STD_OUTPUT_HANDLE);
void* ConsoleManager::context = nullptr;
ConsoleManager::Type ConsoleManager::contextType = T_Null;

void ConsoleManager::WriteObject(GameObject* obj, std::string prefix) {

	std::cout << prefix << " " << obj->name << '\n';
	for (int i = 0; i < obj->children.size(); i++) {
		WriteObject(obj->children[i], '\t' + prefix);
	}
}

ConsoleManager::Command ConsoleManager::IdentifyCommand(std::string str) {
	if (str == "quit") return G_Quit;
	if (str == "fps") return G_FPS;
	if (str == "tree") return G_Tree;
	if (str == "gameobject" || str == "go") return G_GameObject;
	if (str == "transform" || str == "t") return GO_Transform;
	if (str == "parent" || str == "p") return GO_Parent;
	if (str == "destroy" || str == "d") return GO_Destroy;
	return UNKNOWN;
}

void ConsoleManager::ParseCommand(std::string command)
{

	//general commands

	std::vector<std::string> args = Utils::Words(command, ' ');
	int size = args.size();
	if (size == 0) return;

	Command prefix = IdentifyCommand((args)[0]);

	switch (prefix)
	{
	default:
	{
		break;
	}
	case G_Quit:

	{
		SetConsoleTextAttribute(console, 10);
		std::cout << "closing app\n";
		glfwSetWindowShouldClose(window, true);

		break;
	}

	case G_FPS:

	{
		std::cout << Time::fps;

		break;
	}

	case G_Tree:

	{
		SetConsoleTextAttribute(console, 10);
		Scene* scene = Scene::currentScene;
		std::cout << "scene (" << scene->objects.size() << ")\n";
		std::string prefix = "";
		for (int i = 0; i < scene->objects.size(); i++)
		{
			if (scene->objects[i]->parent == NULL)
				WriteObject(scene->objects[i].get(), "`-->");

		}

		break;
	}

	case G_GameObject:

	{
		if (size < 2) {
			SetConsoleTextAttribute(console, 12);
			std::cout << "SyntaxError: gameobject <name>";
			break;
		}
		Scene* scene = Scene::currentScene;
		GameObject* gameobject;
		bool found = false;

		std::queue<GameObject*> queue;

		for (int i = 0; i < scene->objects.size(); i++)
		{
			gameobject = scene->objects[i].get();
			queue.push(gameobject);
		}
		while (queue.size() > 0) {

			gameobject = queue.front();
			queue.pop();

			if ((args)[1] == gameobject->name) {
				SetConsoleTextAttribute(console, 10);
				std::cout << "Selected object '" << gameobject->name << "'";
				found = true;

				context = static_cast<void*>(gameobject);
				contextType = T_GameObject;
				break;
			}
			if (gameobject->children.size() > 0)
			{
				for (GameObject* child : gameobject->children)
					queue.push(child);
			}
		}
		if (!found) {
			SetConsoleTextAttribute(console, 12);
			std::cout << "Object not found";
		}

		break;
	}

	case GO_Transform:
	{
		if (contextType != T_GameObject) {

			SetConsoleTextAttribute(console, 12);
			std::cout << "No GameObject selected";
			break;
		}
		GameObject* gameObject = static_cast<GameObject*>(context);
		PrintMatrix(gameObject->transform);

		break;
	}
	case GO_Parent:
	{
		if (contextType != T_GameObject) {

			SetConsoleTextAttribute(console, 12);
			std::cout << "No GameObject selected";
			break;
		}
		GameObject* gameObject = static_cast<GameObject*>(context);
		if (gameObject->parent == nullptr)
		{
			SetConsoleTextAttribute(console, 12);
			std::cout << "This gameobject is orphan";
			break;
		}
		SetConsoleTextAttribute(console, 10);
		std::cout << gameObject->parent->name << ' ';
		if (size > 1 && ((args)[1] == "s" || (args)[1] == "select")) {
			context = gameObject->parent;
		}

		break;
	}
	case GO_Destroy:
	{
		if (contextType != T_GameObject) {

			SetConsoleTextAttribute(console, 12);
			std::cout << "No GameObject selected";
			break;
		}
		auto gameObject = static_cast<GameObject*>(context);
		DestroyManager::get()->push(gameObject);

		context = nullptr;
		contextType = T_Null;

		break;
	}
	}

	SetConsoleTextAttribute(console, 15);
	std::cout << '\n';
}

void ConsoleManager::Initialize(GLFWwindow* window) {
	ConsoleManager::window = window;
}

template <typename mat>
void ConsoleManager::PrintMatrix(mat matrix)
{
	const int cols = matrix.length();
	const int rows = matrix[0].length();
	std::cout << "+-------";
	for (int j = 0; j < cols; j++) {

		std::cout << "--------";
	}
	for (int i = 0; i < rows; i++) {
		std::cout << "\n|\t";
		for (int j = 0; j < cols; j++) {
			std::string value = std::to_string(matrix[j][i]);
			int difference = value.size() - 7;
			if (difference < 0)
			{
				for (int i = difference; i < 0; i++)
					value += ' ';
			}
			if (difference > 0)
			{
				value = value.substr(0, 7);
			}
			std::cout << value << '\t';
		}
	}
}