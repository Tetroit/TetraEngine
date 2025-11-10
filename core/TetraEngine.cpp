#include "tetrapc.h"

//uncomment this to launch testing script
//#define TETRA_TEST

#ifndef TETRA_TEST

#include "../Core.h"
#include "../applications/ApplicationConfig.h"
#ifndef TETRA_APPLICATION_NAME
#error No application defined
#endif

#define TETRA_STR_HELPER(x) #x
#define TETRA_STR(x) TETRA_STR_HELPER(x)

#define TETRA_APPLICATION_INCLUDE TETRA_STR(../applications/TETRA_APPLICATION_NAME.h)

#include TETRA_APPLICATION_INCLUDE

using namespace TetraEngine;

int main()
{
	if (Core::Initialize() != 0)
		return -1;

	Core::CreateApplication<TETRA_APPLICATION_NAME>();

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
#else

#include "../Test.h"

int main()
{
	return TetraTest::test_main();
}
#endif
