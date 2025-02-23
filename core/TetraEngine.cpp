#include "tetrapc.h"
#include "../Core.h"
#include "../MyApplication.h"

using namespace TetraEngine;

int main()
{
	if (Core::Initialize() != 0)
		return -1;

	Core::CreateApplication<MyApplication>();

	std::thread consoleThread(Core::processConsole);

	while (!glfwWindowShouldClose(Core::glfwManager->window))
	{
		//layers
		Core::Update();
		Core::UpdateOverlay();
		Core::AfterUpdate();
	}
	consoleThread.detach();
	consoleThread.~thread();
	Core::CleanUp();

	return 0;
}
