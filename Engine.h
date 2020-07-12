#pragma once

#ifndef Engine_H
#define Engine_H

#include "glad.h"
#include "glfw3.h"

#include "logger.h"
#include "Renderer.h"
#include "Input.h"
#include "Model.h"

namespace Engine
{

	//Important: First call Instance() before doing anything wiht the class
	class Engine
	{
	public:

		char* project_Name;

		static Engine* Instance() {
			static Engine* instance;

			if (!instance) {
				instance = new Engine();
			}

			return instance;
		}

		int Init(const char* projectPath, const char* projectName);

		void Start();

		void mainLoop();
		Renderer* renderer;
	private:
		//Variables-----------------------------------
		
		Input::Input* input;

		Model* m_model;

		// timing
		float deltaTime = 0.0f;	// time between current frame and last frame
		float lastFrame = 0.0f;

		//Methods-------------------------------------
		Engine() {

		}

		Engine(Engine const&) = delete;
		Engine& operator=(Engine const&) = delete;

		static void glfw_error_callback(int error, const char* description)
		{
			log_printf(log_level_e::LOG_INFO,"ERROR::GLFW	%d: %s\n", error, description);
		}
	};
}

#endif

