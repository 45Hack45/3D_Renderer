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

			for (std::pair<std::string, std::string> shaderPath : *(shaders))
				AddShader(shaderPath.first, shaderPath.second, loadShaders);
		}

		void AddShader(const std::string& shaderName, const std::string& shaderPath, bool loadShader = false) {
			shadersPath[shaderName] = shaderPath;
			if (loadShader) 
				shaders[shaderName] = new Shader(shadersPath[shaderName].c_str());//load Shader
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
				}
				else {
					//Loading shader
					shaders[shaderName] = new Shader(shadersPath[shaderName].c_str());
				}
			}

			return shaders[shaderName];
		}

		/*const Shader& operator[](const std::string& shaderName) {
			return getShader(shaderName);
		}*/

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
			"\n layout(location = 0) in vec3 aPos;"
			"\n uniform mat4 model; uniform mat4 view; uniform mat4 projection;"
			"\n void main(){"
			"\n gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
			"\n }";

		const char* _ErrorShaderFragCode =
			"   #version 330 core"
			"\n out vec4 FragColor;"
			"\n void main(){FragColor = vec4(51, 255, 51, 255)/255;}";
	};
}

#endif

