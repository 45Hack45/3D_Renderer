#include "Material.h"

using namespace std;

namespace Engine
{
	void Material::UpdateMaterialPropeties() {
		if (!m_shader->hasPropeties)
			return;

		for (size_t i = 0; i < m_shader->shaderProperties.size(); i++)
		{
			std::string property = m_shader->shaderProperties[i];
			std::string sVarName = "", varType = "", varName = "";
			char t;

			std::stringstream lineStream(property.c_str());
			std::string values[4];

			lineStream >> t >> varType >> sVarName >> varName;

			AddProperty(varType, sVarName, varName);
		}
	}

	void Material::AddProperty(const std::string& varType, const std::string& sVarName, const std::string& varName) {
		std::string variableName = varName.substr(0, varName.size() - 1);//deleting final ) from the name
		if (varType == "int")
			intProperties[variableName] = pair<string, int>(sVarName, 0);
		else if (varType == "float")
			floatProperties[variableName] = pair<string, float>(sVarName, 0.0f);
		else if (varType == "bool")
			boolProperties[variableName] = pair<string, bool>(sVarName, false);
		else if (varType == "vec2")
			vec2Properties[variableName] = pair<string, glm::vec2>(sVarName, glm::vec2(0.f));
		else if (varType == "vec3")
			vec3Properties[variableName] = pair<string, glm::vec3>(sVarName, glm::vec3(0.f));
		else if (varType == "vec4")
			vec4Properties[variableName] = pair<string, glm::vec4>(sVarName, glm::vec4(0.f));
		else if (varType == "mat4")
			mat4Properties[variableName] = pair<string, glm::mat4>(sVarName, glm::mat4(1.f));//default value is identity

		else if (varType == "sampler2D") {//text2D
			//							index		before incrementing size
			tex2DProperties[variableName] = tex2D.size();
			tex2D.push_back(pair<string, string>(sVarName, ""));
		}
		else {
			log_printf(log_level_e::LOG_INFO, "ERROR::MATERIAL::INCORRECT_VARIABLE_TYPE: %s \n	Shader: %s", varType.c_str(),m_shader->assetName.c_str());
		}
	}

	void Material::sendMaterial2Shader() {
		//int sizeInt, sizeFloat, sizeBool, sizeVec2, sizeVec3, sizeVec4, sizeMat4, sizeTexture2D;

		//-------------------------------------------------Int
		for (auto iter : intProperties) {
			auto element = iter.second;
							//first: name	Second: value
			m_shader->setInt(element.first, element.second);
		}
		//-------------------------------------------------Float
		for (auto iter : floatProperties) {
			auto element = iter.second;
			m_shader->setFloat(element.first, element.second);
		}
		//-------------------------------------------------Bool
		for (auto iter : boolProperties) {
			auto element = iter.second;
			m_shader->setBool(element.first, element.second);
		}
		//-------------------------------------------------Vec2
		for (auto iter : vec2Properties) {
			auto element = iter.second;
			m_shader->setVector(element.first, element.second);
		}
		//-------------------------------------------------Vec3
		for (auto iter : vec3Properties) {
			auto element = iter.second;
			m_shader->setVector(element.first, element.second);
		}
		//-------------------------------------------------Vec4
		for (auto iter : vec4Properties) {
			auto element = iter.second;
			m_shader->setVector(element.first, element.second);
		}
		//-------------------------------------------------Mat4
		for (auto iter : mat4Properties) {
			auto element = iter.second;
			m_shader->setMat4(element.first, element.second);
		}
		//-------------------------------------------------Tex2D
		for (size_t i = 0; i < tex2D.size(); i++){
			//												texture name
			Texture* texture = m_textureManager->getTexture(tex2D[i].second,false);
			if (texture->nChanels() == 4)
				isTransparent = true;

			if(texture)
				texture->bind(i);
		}
	}

	void Material::printPropNames() {
		log_printf(LOG_DEBUG, "\n\n-------------------------------------------------Printing Material %s properties:", name);
		//-------------------------------------------------Int
		if(intProperties.size() > 0)
			log_message(LOG_DEBUG, "\n	-------------------------------------------------Int");
		for (auto iter : intProperties) {
			log_printf(log_level_e::LOG_DEBUG, "	Property Name: %s", iter.first.c_str());
		}

		//-------------------------------------------------Float
		if (floatProperties.size() > 0)
			log_message(LOG_DEBUG, "\n	-------------------------------------------------Float");
		for (auto iter : floatProperties) {
			log_printf(log_level_e::LOG_DEBUG, "	Property Name: %s", iter.first.c_str());
		}

		//-------------------------------------------------Bool
		if (boolProperties.size() > 0)
			log_message(LOG_DEBUG, "\n	-------------------------------------------------Bool");
		for (auto iter : boolProperties) {
			log_printf(log_level_e::LOG_DEBUG, "	Property Name: %s", iter.first.c_str());
		}

		//-------------------------------------------------Vec2
		if (vec2Properties.size() > 0)
			log_message(LOG_DEBUG, "\n	-------------------------------------------------Vec2");
		for (auto iter : vec2Properties) {
			log_printf(log_level_e::LOG_DEBUG, "	Property Name: %s", iter.first.c_str());
		}

		//-------------------------------------------------Vec3
		if (vec2Properties.size() > 0)
			log_message(LOG_DEBUG, "\n	-------------------------------------------------Vec3");
		for (auto iter : vec3Properties) {
			log_printf(log_level_e::LOG_DEBUG, "	Property Name: %s", iter.first.c_str());
		}

		//-------------------------------------------------Vec4
		if (vec4Properties.size() > 0)
			log_message(LOG_DEBUG, "\n	-------------------------------------------------Vec4");
		for (auto iter : vec4Properties) {
			log_printf(log_level_e::LOG_DEBUG, "	Property Name: %s", iter.first.c_str());
		}

		//-------------------------------------------------Mat4
		if (mat4Properties.size() > 0)
			log_message(LOG_DEBUG, "\n	-------------------------------------------------Mat4");
		for (auto iter : mat4Properties) {
			log_printf(log_level_e::LOG_DEBUG, "	Property Name: %s", iter.first.c_str());
		}

		//-------------------------------------------------Tex2D
		if (tex2DProperties.size() > 0)
			log_message(LOG_DEBUG, "\n	-------------------------------------------------Texture2D");
		for (auto iter : tex2DProperties) {
			log_printf(log_level_e::LOG_DEBUG, "	Property Name: %s", iter.first.c_str());
		}
	}
}
