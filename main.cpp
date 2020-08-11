#include "glad.h"
#include "glfw3.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "logger.h"

//#include "TestRender.hpp"

#include "Engine.h"

int main() {
	set_log_level(log_level_e::LOG_DEBUG);

	Engine::Engine* engine = Engine::Engine::Instance();

	int error = engine->Init("./EngineProject", "Test_Project");

	if (error < 0)
	{
		system("pause");
		return error;
	}

	engine->Start();

	engine->mainLoop();


	//return testMouseInput();
	//test_render();

	return 0;
}


