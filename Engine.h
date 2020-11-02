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
		bool gammaCorrection = false;
		Renderer* renderer;
		Scene* scene;//Current scene

	private:
		//Variables-----------------------------------
		Editor* editor;

		//Managers
		Input::Input* input;
		ShaderManager* shaderManager;
		TextureManager* textureManager;
		ModelManager* modelManager;
		MaterialManager* materialManager;

		Model* m_model;
		Model* m_model2;
		

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

		void imgui_beginFrame_() {
			//GUI window
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		void imgui_RenderFrame() {
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void sendLightInfo2Shader(Shader* shader, const std::vector<LightSource_Point>& pointLights, std::vector<LightSource_Spot>& spotLights, const LightSource_Directional& dirLight);
		void sendProjectionInfo2Shader(Shader* shader, Camera* cam);
		void sendCascadeShadowMapInfo2Shader(Shader* shader, Camera* cascadeCam0, Camera* cascadeCam1, Camera* cascadeCam2, Camera* cascadeCam3);
		void RenderCascadeShadowmaps(Shader* shader, glm::vec3 lightPos, Camera* cascadeCam0, Camera* cascadeCam1, Camera* cascadeCam2, Camera* cascadeCam3);

		void drawScene(Camera* cam);

		//draws a full screen quad with a texture.
		//if useFullScreenQuadShader is false will use the previously binded shader (useful for sending shader info for postprocessing effects)
		void drawFullScreenQuad(unsigned int textureID,  unsigned int textureAtachment = 0, bool useFullScreenQuadShader = true);

		// A typedef to make for easier reading
		typedef bool (*RenderEntity_Callback)(Entity*, Shader*);

		//Takes a function that it's called for every entity and if the function returns false the entity is not rendered
		void renderPass(Camera* cam, RenderEntity_Callback func = [](Entity*, Shader*) {return true; });

		void renderPass(Camera* cam, Shader* shader, RenderEntity_Callback func = [](Entity*, Shader*) {return true; });;
	};
}

#endif

