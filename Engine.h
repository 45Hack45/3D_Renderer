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


#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "ModelManager.hpp"
#include "MaterialManager.hpp"

namespace Engine
{
	const int _PI = 3.14159265359;

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

		//Creates an entity and returns a pointer to the new entity
		Entity* createEntity();
		Entity* createEntity(Entity* parent);
		Entity* createModelEntities(Model* model);
		
		bool useTransparency = false;

	private:
		//Variables-----------------------------------
		Renderer* renderer;
		Input::Input* input;
		ShaderManager* shaderManager;
		TextureManager* textureManager;
		ModelManager* modelManager;
		MaterialManager* materialManager;

		Model* m_model;
		Entity m_scene;
		std::vector<Entity*> entities;

		LightSource_Directional m_directionalLight;
		std::vector<LightSource_Point> m_pointLights;
		std::vector<LightSource_Spot> m_spotLights;

		bool show_light_gui;
		float dirLight_rotation_X = 0;

		// timing
		float deltaTime = 0.0f;	// time between current frame and last frame
		float lastFrame = 0.0f;

		//Methods-------------------------------------
		Engine(): m_directionalLight(Color::White, 1, glm::vec3(0,1,0)){
			m_directionalLight.m_intensity = 1;;
			m_pointLights.reserve(10);
			m_spotLights.reserve(10);
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
		void renderGui();

		void loadModelMaterials(Model* model);

		void createModelEntity(Model* model, aiNode* parentNode, Entity* parent);
		void createModelEntityMeshes(Model* model, aiNode* parentNode, Entity* parent);
	};
}

#endif

