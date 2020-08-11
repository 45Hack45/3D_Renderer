#pragma once

#ifndef Engine_H
#define Engine_H

#include "glad.h"
#include "glfw3.h"

#include "logger.h"
#include "Renderer.h"
#include "Input.h"
#include "Model.h"
#include "LightSource.h"
#include "Entity.h"
#include "Camera.h"
#include "Scene.h"
#include "Editor.h"


#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "ModelManager.hpp"
#include "MaterialManager.hpp"

namespace Engine
{

	//Important: First call Instance() before doing anything wiht the class
	class Engine
	{
	public:

		char* project_Name;

		//Methods-------------------------------------
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
		

		//Variables-----------------------------------
		bool useTransparency = false;
		Renderer* renderer;

	private:
		//Variables-----------------------------------
		Scene* scene;//Current scene
		Editor* editor;

		//Managers
		Input::Input* input;
		ShaderManager* shaderManager;
		TextureManager* textureManager;
		ModelManager* modelManager;
		MaterialManager* materialManager;

		Model* m_model;
		

		// timing
		float deltaTime = 0.0f;	// time between current frame and last frame
		float lastFrame = 0.0f;
		float currentFrame = 0.0f;

		//Methods-------------------------------------
		Engine(){
		}

		Engine(Engine const&) = delete;
		Engine& operator=(Engine const&) = delete;

		static void glfw_error_callback(int error, const char* description)
		{
			log_printf(log_level_e::LOG_INFO,"ERROR::GLFW	%d: %s\n", error, description);
		}

		void sendLightInfo2Shader(Shader& shader, const std::vector<LightSource_Point>& pointLights, std::vector<LightSource_Spot>& spotLights, const LightSource_Directional& dirLight);
		void sendProjectionInfo2Shader(Shader* shader, Camera* cam);

		void drawScene(Camera* cam);
	};
}

#endif

