#include "glad.h"
#include "glfw3.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Engine.h"

int main() {
	set_log_level(log_level_e::LOG_DEBUG);

	Engine::Engine* engine = Engine::Engine::Instance();

	int error = engine->Init("C:/Users/Hack45/source/repos/Renderer3D/EngineProject", "Test_Project");

	if (error < 0)
	{
		return error;
	}

	engine->mainLoop();

	return 0;
}
