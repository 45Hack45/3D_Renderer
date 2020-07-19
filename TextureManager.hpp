#pragma once

#ifndef Texture_MANAGER_H_
#define Texture_MANAGER_H_
#include "Texture.h"

#include <string>
#include <unordered_map>
#include "IO.h"

namespace Engine
{
	class TextureManager
	{
	public:
		//Methods-------------------------------------
		static TextureManager* Instance() {
			static TextureManager* instance;

			if (!instance)
				instance = new TextureManager();

			return instance;
		}

		void Init(bool loadShaders = false) {

			std::vector< std::pair<std::string, std::string>>* textures = IO::getTexturesPaths();

			for (std::pair<std::string, std::string> texturePath : *(textures))
				AddTexture(texturePath.first, texturePath.second, loadShaders);
		}

		void AddTexture(const std::string& textureName, const std::string& texturePath, bool loadTexture = false) {
			texturesPath[textureName] = texturePath;
			if (loadTexture) 
				textures[textureName] = new Texture(texturesPath[textureName].c_str());//load texture
		}

		Texture* getTexture(const std::string& textureName) {
			if (textures.find(textureName) == textures.end())
			{//Shader not found

				//Trying to load the shader
				if (texturesPath.find(textureName) == texturesPath.end()) {
					//Shader path not found
					log_error("ERROR::Texture_MANAGER::Texture file not found.");
					texturesPath[textureName] = "_ERROR_Texture";	
				}
				else {
					//Loading shader
					textures[textureName] = new Texture(texturesPath[textureName].c_str());
				}
			}

			return textures[textureName];
		}

		/*const Shader& operator[](const std::string& shaderName) {
			return getShader(shaderName);
		}*/

		//Variables-----------------------------------

	private:

		//Methods-------------------------------------
		TextureManager(){

		}
		TextureManager(TextureManager const&) = delete;
		TextureManager& operator=(TextureManager const&) = delete;
		~TextureManager() {};


		//Variables-----------------------------------
		std::unordered_map<std::string, std::string> texturesPath;
		std::unordered_map<std::string, Texture*> textures;
	};
}

#endif
