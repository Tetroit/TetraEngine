#pragma once

class TetraTest
{
private:
	TetraTest() = default;
public:
	static int test_main();
	static void test_processInput(GLFWwindow* window);
	static void test_setup();
	static void test_update();
};
