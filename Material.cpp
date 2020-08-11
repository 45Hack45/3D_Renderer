#include "Material.h"
#include "Editor.h"

#define _nameMaxSize 64

using namespace std;

namespace Engine
{
	bool Material::UpdateMaterialPropeties() {

		//Clear maps
		intProperties.clear();
		floatProperties.clear();
		boolProperties.clear();
		vec2Properties.clear();
		vec3Properties.clear();
		vec4Properties.clear();
		mat4Properties.clear();
		isColorProperties.clear();
		tex2DProperties.clear();
		tex2D.clear();


		if (!m_shader->hasPropeties)
			return false;

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

		return true;
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
		else if (varType == "vec3") {
			vec3Properties[variableName] = pair<string, glm::vec3>(sVarName, glm::vec3(0.f));
			isColorProperties[variableName] = pair<string, bool>(sVarName, false);
		}
		else if (varType == "vec4") {
			vec4Properties[variableName] = pair<string, glm::vec4>(sVarName, glm::vec4(0.f));
			isColorProperties[variableName] = pair<string, bool>(sVarName, false);
		}
		else if (varType == "color" || varName == "color4") {
			vec4Properties[variableName] = pair<string, glm::vec4>(sVarName, glm::vec4(0.f));
			isColorProperties[variableName] = pair<string, bool>(sVarName, true);
		}
		else if (varName == "color3") {
			vec3Properties[variableName] = pair<string, glm::vec3>(sVarName, glm::vec3(0.f));
			isColorProperties[variableName] = pair<string, bool>(sVarName, true);
		}
		else if (varType == "mat4")
			mat4Properties[variableName] = pair<string, glm::mat4>(sVarName, glm::mat4(1.f));//default value is identity
		else if (varType == "sampler2D") {//text2D
			//							index		before incrementing size
			tex2DProperties[variableName] = tex2D.size();
			tex2D.push_back(pair<string, Texture*>(sVarName, m_textureManager->getTexture("Engine_AssetGUI_Grey_Texture")));
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
			Texture* texture = tex2D[i].second;
			if (texture) {
				if (texture->nChanels() == 4)
					isTransparent = true;
				texture->bind(i);
			}
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

		//-------------------------------------------------Color3

		//-------------------------------------------------Color4
	}
	void Material::drawEditorGUI_Properties()
	{
		char* tName = new char[_nameMaxSize];

		tName = (char*)name.c_str();

		ImGui::InputText("Material", tName, _nameMaxSize);
		name = tName;

		//-------------------------------------------------Shader
		Shader* tShader = Editor::GUI_PropertySelector("Shader", m_shader);

		if (m_shader != tShader) {
			m_shader = tShader;

			UpdateMaterialPropeties();
		}

		//-------------------------------------------------Int
		auto iterI = intProperties.begin();
		auto endI = intProperties.end();
		while (iterI != endI){
			ImGui::DragInt(iterI->first.c_str(), &iterI->second.second);
			iterI++;
		}

		//-------------------------------------------------Float
		auto iterF = floatProperties.begin();
		auto endF = floatProperties.end();
		while (iterF != endF) {
			ImGui::DragFloat(iterF->first.c_str(), &iterF->second.second);
			iterF++;
		}
		//-------------------------------------------------Bool
		auto iterB = boolProperties.begin();
		auto endB = boolProperties.end();
		while (iterB != endB) {
			ImGui::Checkbox(iterB->first.c_str(), &iterB->second.second);
			iterB++;
		}

		//-------------------------------------------------Vec2
		auto iterV2 = vec2Properties.begin();
		auto endV2 = vec2Properties.end();
		while (iterV2 != endV2) {
			ImGui::DragFloat2(iterV2->first.c_str(), (float*)&iterV2->second.second);
			iterV2++;
		}
		//-------------------------------------------------Vec3
		auto iterV3 = vec3Properties.begin();
		auto endV3 = vec3Properties.end();
		while (iterV3 != endV3) {
			if (isColorProperties[(*iterV3).first].second)//Check if is color
				ImGui::DragFloat3(iterV3->first.c_str(), (float*)&iterV3->second.second);
			else
				ImGui::ColorEdit3(iterV3->first.c_str(), (float*)&iterV3->second.second);
			iterV3++;
		}
		//-------------------------------------------------Vec4
		auto iterV4 = vec4Properties.begin();
		auto endV4 = vec4Properties.end();
		while (iterV4 != endV4) {
			if (isColorProperties[(*iterV4).first].second)//Check if is color
				ImGui::DragFloat4(iterV4->first.c_str(), (float*)&iterV4->second.second);
			else
				ImGui::ColorEdit4(iterV4->first.c_str(), (float*)&iterV4->second.second);
			iterV4++;
		}
		//-------------------------------------------------Mat4
		auto iterM4 = mat4Properties.begin();
		auto endM4 = mat4Properties.end();
		while (iterM4 != endM4) {
			auto value = glm::value_ptr(iterM4->second.second);
			ImGui::DragFloat4(iterM4->first.c_str(), (float*)value);
			iterM4++;
		}

		//TODO: Texture 2D
		//-------------------------------------------------Tex2D-
		auto iterTex2D = tex2D.begin();
		auto endTex2D = tex2D.end();
		while (iterTex2D != endTex2D) {
			//			Texture
			iterTex2D->second = Editor::GUI_PropertySelector(iterTex2D->first.c_str(), iterTex2D->second);
			iterTex2D++;
		}
	}
}
