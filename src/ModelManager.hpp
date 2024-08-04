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

			std::cout << "Adding models:" << std::endl;
			std::cout << "	Number of models: " << models->size() << std::endl;
			for (std::pair<std::string, std::string> modelPath : *(models)){
				std::cout << "		Adding model: " << modelPath.first << std::endl;
				AddModel(modelPath.first, modelPath.second, loadModels);
			}
		}

		void AddModel(const std::string& modelName, const std::string& modelPath, bool loadModel = false) {
			std::cout << "			Model path: " << modelPath << std::endl;
			modelsPath[modelName] = modelPath;
			if (loadModel) {
				std::cout << "			Loading model: " << modelPath << std::endl;
				// TODO: Window not opening when this code is discommented!!!!!!
				// maybe it's crashing on startup
				// TODO: When the program crashes no error is shown (windows crash popup)!!!!!!!!!
				// models[modelName] = new Model(modelsPath[modelName].c_str(), modelName); //load model
				// models[modelName]->loadFile();
			}
		}

		Model* getModel(const std::string& modelName, bool loadModel = true, bool flipUVs = false) {
			if (models.find(modelName) == models.end())
			{ //Model not found

				//Trying to load the model
				if (modelsPath.find(modelName) == modelsPath.end()) {
					//Model path not found
					log_error("ERROR::MODEL_MANAGER::Model file not found.");
					modelsPath[modelName] = "_ERROR_MODEL";
					return nullptr;
				}
				else {
					//Loading model
					models[modelName] = new Model(modelsPath[modelName].c_str(), modelName, flipUVs);
					if(loadModel)
						models[modelName]->loadFile();
				}
			}

			return models[modelName];
		}


		auto Begin() {
			return models.begin();
		}
		auto End() {
			return models.end();
		}

		auto BeginP() {
			return modelsPath.begin();
		}
		auto EndP() {
			return modelsPath.end();
		}

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

