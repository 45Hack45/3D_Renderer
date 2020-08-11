#pragma once

#ifndef Texture_MANAGER_H_
#define Texture_MANAGER_H_
#include "Texture.h"

#include <string>
#include <unordered_map>
#include "IO.h"

#define _ERROR_Texture "./rcs/_EngineGui/#HIDE#InvisibleRsources/Engine_AssetGUI_Grey_Texture.jpg"

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

		void Init(bool loadTextures = false) {

			std::vector< std::pair<std::string, std::string>>* textures = IO::getTexturesPaths();

			for (std::pair<std::string, std::string> texturePath : *(textures))
				AddTexture(texturePath.first, texturePath.second, loadTextures);
		}

		void AddTexture(const std::string& textureName, const std::string& texturePath, bool loadTexture = false) {
			texturesPath[textureName] = texturePath;
			if (loadTexture) 
				textures[textureName] = new Texture(texturesPath[textureName].c_str());//load texture
		}

		Texture* getTexture(const std::string& textureName, bool flipY = true) {
			if (textures.find(textureName) == textures.end())
			{//Texture not found

				//Trying to load the texture
				if (texturesPath.find(textureName) == texturesPath.end()) {
					//texture path not found
					log_error("ERROR::TEXTURE_MANAGER::Texture file not found.");
					texturesPath[textureName] = _ERROR_Texture;
					return nullptr;
				}
				else {
					//Loading texture
					textures[textureName] = new Texture(texturesPath[textureName].c_str(), GL_RGB, flipY);
					textures[textureName]->assetName = textureName;
				}
			}
			return textures[textureName];
		}

		auto Begin() {
			return textures.begin();
		}
		auto End() {
			return textures.end();
		}
		auto BeginP() {
			return texturesPath.begin();
		}
		auto EndP() {
			return texturesPath.end();
		}

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

