#ifndef SHADER_H
#define SHADER_H

#include "include/glad.h"

#include "logger.h"
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "include/glm/glm.hpp"

class Shader
{
public:
	
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath)
	{

		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			perror("	ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();


		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader Program
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertex);
		glAttachShader(m_ID, fragment);
		glLinkProgram(m_ID);
		checkCompileErrors(m_ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	//TODO:
	// constructor generates the shader on the fly frome one single file
	// ------------------------------------------------------------------------
	Shader(const char* shaderPath) {
		log_printf(log_level_e::LOG_INFO,"ERROR::SHADER::CONSTRUCTOR_NOT_IMPLEMENTED\n SHADER: %s", shaderPath);
	}

	~Shader() {
		glDeleteProgram(m_ID);//Free resources
	}

	unsigned int ID()const { return m_ID; }

	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(m_ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
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
		glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x,y);
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
				log_printf(log_level_e::LOG_INFO, "	ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n -- --------------------------------------------------- -- \n",type.c_str(),infoLog);
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
};

#endif

