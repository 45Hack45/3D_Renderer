#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <unordered_map>

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "TextureManager.hpp"
#include "Component.h"
#include "Color.h"

namespace Engine
{
	class Material : public Component
	{
	public:
		//Methods-------------------------------------

		Material(Shader* shader) {
			name = "Material";
			m_shader = shader;
			m_textureManager = TextureManager::Instance();//Assuming it has already been initialized
			UpdateMaterialPropeties();
		}

		void sendMaterial2Shader();

		void printPropNames();


		void drawEditorGUI_Properties();

		//Setters

		// ------------------------------------------------------------------------
		bool setInt(std::string property, int value) { 
			try{
				intProperties.at(property).second = value;
			}catch (const std::exception& e){
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}
		// ------------------------------------------------------------------------
		bool setBool(std::string property, bool value) { 
			try {
				boolProperties[property].second = value;
			}
			catch (const std::exception& e) {
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}
		// ------------------------------------------------------------------------
		bool setFloat(std::string property, float value) { 
			try {
				floatProperties.at(property).second = value;
			}
			catch (const std::exception& e) {
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}
		// ------------------------------------------------------------------------
		bool setVec2(std::string property, glm::vec2 value) { 
			
			try {
				vec2Properties.at(property).second = value;
			}
			catch (const std::exception& e) {
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}
		bool setVec2(std::string property, float x, float y) { 
			try {
				vec2Properties.at(property).second = glm::vec2(x, y);
			}
			catch (const std::exception& e) {
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}
		// ------------------------------------------------------------------------
		bool setVec3(std::string property, glm::vec3 value, bool isColor = false) {
			try {
				vec3Properties.at(property).second = value;
				isColorProperties.at(property).second = isColor;
			}
			catch (const std::exception& e) {
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}
		bool setVec3(std::string property, float x, float y, float z, bool isColor = false) {
			try {
				vec3Properties.at(property).second = glm::vec3(x, y, z);
				isColorProperties.at(property).second = isColor;
			}
			catch (const std::exception& e) {
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}
		// ------------------------------------------------------------------------
		bool setVec4(std::string property, glm::vec4 value, bool isColor = false) {
			try {
				vec4Properties.at(property).second = value;
				isColorProperties.at(property).second = isColor;
			}
			catch (const std::exception& e) {
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}
		bool setVec4(std::string property, float x, float y, float z, float w, bool isColor = false) {
			try {
				vec4Properties.at(property).second = glm::vec4(x, y, z, w);
				isColorProperties.at(property).second = isColor;
			}
			catch (const std::exception& e) {
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}
		// ------------------------------------------------------------------------
		bool setColor(std::string property, const Color& color) {
			return setVec4(property, color.color_vect, true);
		}
		// ------------------------------------------------------------------------
		bool setMat4(std::string property, glm::mat4 value) {
			try {
				mat4Properties.at(property).second = value;
			}
			catch (const std::exception& e) {
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}
		// ------------------------------------------------------------------------
		bool setTexture2D(std::string property, std::string value, bool flipTeture = false) {
			try {
				int index = tex2DProperties.at(property);

				//tex2D[index].second = value;
				tex2D[index].second = m_textureManager->getTexture(value, flipTeture);
			}
			catch (const std::exception& e) {
				log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::FAILED_TO_SET_PROPERTY: %s\n	Material: %s	Property %s", e.what(), name, property.c_str());
				return false;
			}
			return true;
		}


		//Variables-----------------------------------
		Shader* m_shader;
		bool isTransparent = false;

	private:
		//Variables-----------------------------------
		TextureManager* m_textureManager;

		//					varName					sVarName	value
		std::unordered_map<std::string, std::pair<std::string, int>>		intProperties;
		std::unordered_map<std::string, std::pair<std::string, float>>		floatProperties;
		std::unordered_map<std::string, std::pair<std::string, bool>>		boolProperties;
		std::unordered_map<std::string, std::pair<std::string, glm::vec2>>	vec2Properties;
		std::unordered_map<std::string, std::pair<std::string, glm::vec3>>	vec3Properties;
		std::unordered_map<std::string, std::pair<std::string, glm::vec4>>	vec4Properties;
		std::unordered_map<std::string, std::pair<std::string, glm::mat4>>	mat4Properties;
		std::unordered_map<std::string, std::pair<std::string, bool>>		isColorProperties;//Stores if a vec3 or vec4 is a color to display it in the property window as a color

		//Texture 2D is separated into two variables to ensure that the order of insertion is maintained. The texture index is used as TextureUnit

		//					varName		tex2D index
		std::unordered_map<std::string, int>								tex2DProperties;
		//						sVarName	texture
		std::vector<std::pair<std::string, Texture_Asset*>>						tex2D;

		//Methods-------------------------------------

		bool UpdateMaterialPropeties();

		void AddProperty(const std::string& varType, const std::string&  sVarName, const std::string& varName);
	};
}

#endif
