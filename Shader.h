#pragma once

#ifndef SHADER_H
#define SHADER_H

#include "include/glad.h"

#include "logger.h"

#include "include/glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Asset.h"

class Shader : public Engine::Asset
{
public:

	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath, bool isFile = true);

	// constructor generates the shader on the fly frome one single file
	// The convention is that every shader type start with: //#Begin_Type and ends with: //#End_Type
	//Example: //#Begin_vert ... //#End_vert	//#Begin_frag ... //#End_frag
	//Everything out of Begin...End won't be compiled
	Shader(const char* shaderPath): Asset(shaderPath, shaderPath) {
		loadFile();
	}

	Shader(const std::string& shaderPath, const std::string& shaderName): Asset(shaderPath,shaderName) {//Constructor for the manager
		loadFile();
	}

	~Shader() {
		glDeleteProgram(m_ID);//Free resources
	}

	unsigned int ID()const { return m_ID; }

	void loadFile();

	// activate the shader
	// ------------------------------------------------------------------------
	void use()const
	{
		glUseProgram(m_ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (value) ? 1 : 0);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
	}
	void setFloat(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
	}
	void setFloat(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, glm::mat4 matrix) {
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
	// ------------------------------------------------------------------------
	void setVector(const std::string& name, glm::vec2 value) const
	{
		glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y);
	}
	void setVector(const std::string& name, glm::vec3 value) const
	{
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
	}
	void setVector(const std::string& name, glm::vec4 value) const
	{
		glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z, value.w);
	}

	bool errorOnLoad = false;

	bool hasPropeties = false;
	std::vector<std::string> shaderProperties;

private:

	unsigned int m_ID;

	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				log_printf(log_level_e::LOG_INFO, "	ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- \n", type.c_str(), infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				log_printf(log_level_e::LOG_INFO, "	ERROR::PROGRAM_LINKING_ERROR of type: %s\n %s\n -- --------------------------------------------------- -- \n", type.c_str(), infoLog);
			}
		}
	}

	void getProperties(const std::string& shaderCode);

};

#endif

