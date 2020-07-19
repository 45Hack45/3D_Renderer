#include "glad.h"
#include "glfw3.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "logger.h"

//#include "TestRender.hpp"

#include "Engine.h"

/*void callbackMouse(GLFWwindow* window, double xpos, double ypos) {

	std::cout << xpos << ",	" << ypos << std::endl;
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "ERROR::GLFW	%d: %s\n", error, description);
}

int testMouseInput() {

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Test mouse input", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Seting window
	glfwMakeContextCurrent(window);

	//Init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800,600);//Screen size

	glfwSetCursorPosCallback(window, callbackMouse);

	while (!glfwWindowShouldClose(window))
	{
		
		glfwSwapBuffers(window);//Update screen
		glfwPollEvents();//check events
	}
}*/

int main() {
	set_log_level(log_level_e::LOG_DEBUG);

	Engine::Engine* engine = Engine::Engine::Instance();

	int error = engine->Init("C:/Users/Hack45/source/repos/Renderer3D/EngineProject", "Test_Project");

	if (error < 0)
	{
		return error;
	}

	engine->Start();

	engine->mainLoop();


	//return testMouseInput();
	//test_render();

	return 0;
}


