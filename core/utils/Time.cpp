#include "tetrapc.h"
#include "Time.h"
#include <GLFW/glfw3.h>

using namespace TetraEngine;

float Time::time = 0;
float Time::prevTime = 0;
float Time::deltaTime;
float Time::sdeltaTime = 0;
float Time::fps = 0;
void Time::Update()
{
	prevTime = time;
	time = glfwGetTime();
	deltaTime = time - prevTime;
	sdeltaTime = (sdeltaTime + deltaTime) / 2;
	fps = (fps + 3 / deltaTime) / 4;
}