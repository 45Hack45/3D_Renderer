#pragma once

#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_
#include "Shader.h"

#include <string>
#include <unordered_map>
#include "IO.h"

namespace Engine
{
	class ShaderManager
	{
	public:
		//Methods-------------------------------------
		static ShaderManager* Instance() {
			static ShaderManager* instance;

			if (!instance)
				instance = new ShaderManager();

			return instance;
		}

		void Init(bool loadShaders = false) {
			
			std::vector< std::pair<std::string, std::string>>* shaders = IO::getShaderPaths();
			std::vector< std::pair<std::string, std::string>>* cShaders = IO::getComputeShaderPaths();

			for (std::pair<std::string, std::string> shaderPath : *(shaders))
				AddShader(shaderPath.first, shaderPath.second, loadShaders);

			for (std::pair<std::string, std::string> shaderPath : *(cShaders))
				AddComputeShader(shaderPath.first, shaderPath.second);
		}

		void AddShader(const std::string& shaderName, const std::string& shaderPath, bool loadShader = false) {
			shadersPath[shaderName] = shaderPath;
			if (loadShader)
				shaders[shaderName] = new Shader(shaderName, shadersPath[shaderName]);//load Shader
		}
		void AddShader(const std::string& shaderName, Shader* shader) {
			shadersPath[shaderName] = shader->filePath;
			shaders[shaderName] = shader;
		}
		void AddComputeShader(const std::string& shaderName, const std::string& shaderPath) {
			shadersPath[shaderName] = shaderPath;
			shaders[shaderName] = new ComputeShader(shadersPath[shaderName], shaderName);//load Shader
		}

		Shader* getShader(const std::string& shaderName) {
			if (shaders.find(shaderName) == shaders.end())
			{//Shader not found

				//Trying to load the shader
				if (shadersPath.find(shaderName) == shadersPath.end()) {
					//Shader path not found
					log_error("ERROR::SHADER_MANAGER::Shader file not found.");
					shadersPath[shaderName] = "_ERROR_SHADER";	
					shaders[shaderName] = _ErrorShader;
					return nullptr;
				}
				else {
					//Loading shader
					shaders[shaderName] = new Shader(shadersPath[shaderName], shaderName);
				}
			}

			return shaders[shaderName];
		}

		auto Begin() {
			return shaders.begin();
		}
		auto End() {
			return shaders.end();
		}
		auto BeginP() {
			return shadersPath.begin();
		}
		auto EndP() {
			return shadersPath.end();
		}

		//Variables-----------------------------------

		Shader* _ErrorShader;

	private:

		//Methods-------------------------------------
		ShaderManager(){

			_ErrorShader = new Shader(_ErrorShaderVertCode, _ErrorShaderFragCode, false);
		}
		ShaderManager(ShaderManager const&) = delete;
		ShaderManager& operator=(ShaderManager const&) = delete;
		~ShaderManager() {};


		//Variables-----------------------------------
		std::unordered_map<std::string, std::string> shadersPath;
		std::unordered_map<std::string, Shader*> shaders;


		const char* _ErrorShaderVertCode =
			"#version 330 core"
			"\n//#Begin_prop"
			"\n//#End_prop"
			"\n layout(location = 0) in vec3 aPos;"
			"\n uniform mat4 model; uniform mat4 view; uniform mat4 projection;"
			"\n void main(){"
			"\n gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
			"\n }";

		const char* _ErrorShaderFragCode =
			"   #version 330 core"
			"\n//#Begin_prop"
			"\n//#End_prop"
			"\n out vec4 FragColor;"
			"\n void main(){FragColor = vec4(51, 255, 51, 255)/255;}";
	};
}

#endif

