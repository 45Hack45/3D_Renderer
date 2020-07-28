#pragma once

#ifndef MODEL_MANAGER_H_
#define MODEL_MANAGER_H_
#include "Model.h"

#include <string>
#include <unordered_map>
#include "IO.h"

namespace Engine
{
	class ModelManager
	{
	public:
		//Methods-------------------------------------
		static ModelManager* Instance() {
			static ModelManager* instance;

			if (!instance)
				instance = new ModelManager();

			return instance;
		}

		void Init(bool loadModels = false) {
			
			std::vector< std::pair<std::string, std::string>>* models = IO::getModelPaths();

			for (std::pair<std::string, std::string> modelPath : *(models))
				AddModel(modelPath.first, modelPath.second, loadModels);
		}

		void AddModel(const std::string& modelName, const std::string& modelPath, bool loadModel = false) {
			modelsPath[modelName] = modelPath;
			if (loadModel) {
				models[modelName] = new Model(modelsPath[modelName].c_str(), modelName);//load model
				models[modelName]->loadFile();
			}
		}

		Model* getModel(const std::string& modelName, bool loadModel = true) {
			if (models.find(modelName) == models.end())
			{//Shader not found

				//Trying to load the shader
				if (modelsPath.find(modelName) == modelsPath.end()) {
					//Shader path not found
					log_error("ERROR::SHADER_MANAGER::Shader file not found.");
					modelsPath[modelName] = "_ERROR_MODEL";
					return nullptr;
				}
				else {
					//Loading shader
					models[modelName] = new Model(modelsPath[modelName].c_str(), modelName);
					if(loadModel)
						models[modelName]->loadFile();
				}
			}

			return models[modelName];
		}

		/*const Shader& operator[](const std::string& shaderName) {
			return getShader(shaderName);
		}*/

		//Variables-----------------------------------

	private:

		//Methods-------------------------------------
		ModelManager(){
		}
		ModelManager(ModelManager const&) = delete;
		ModelManager& operator=(ModelManager const&) = delete;
		~ModelManager() {};


		//Variables-----------------------------------
		std::unordered_map<std::string, std::string> modelsPath;
		std::unordered_map<std::string, Model*> models;
	};
}

#endif

