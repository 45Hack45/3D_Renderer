#include "Engine.h"

#include "IO.hpp"
#include <stdlib.h>

namespace Engine
{
	int Engine::Init(const char* projectPath, const char* projectName) {
		log_message(log_level_e::LOG_DEBUG, "Initializing Engine");
		IO::projectPath = projectPath;

		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
			return -1;

		renderer = Renderer::Instance();

		//Init renderer
		int error = renderer->Init(projectName);

		//Init glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			log_error("ERROR::ENGINE:FAILED_TO_INIT_GLAD\n");
			return -1;
		}
		log_message(log_level_e::LOG_INFO, "Engine Initialized\n");
	}

	void Engine::mainLoop() {
		log_message(log_level_e::LOG_DEBUG, "Starting Main Loop\n");
		while (!glfwWindowShouldClose(renderer->getWindow()))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			renderer->ClearScreen(Color::Grey);

			glfwPollEvents();//check events
			glfwSwapBuffers(renderer->getWindow());//Update screen
		}
	}
}