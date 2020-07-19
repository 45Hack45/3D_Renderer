#include "Shader.h"

Shader::Shader(const char* vertexShader, const char* fragmentShader, bool isFile)
{

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	const char* vShaderCode;
	const char* fShaderCode;

	if (isFile)
	{
		try
		{
			// open files
			vShaderFile.open(vertexShader);
			fShaderFile.open(fragmentShader);
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
			errorOnLoad = true;
		}

		vShaderCode = vertexCode.c_str();
		fShaderCode = fragmentCode.c_str();
	}
	else {
		vShaderCode = vertexShader;
		fShaderCode = fragmentShader;
	}



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

Shader::Shader(const char* shaderPath) {
	//log_printf(log_level_e::LOG_INFO, "ERROR::SHADER::CONSTRUCTOR_NOT_IMPLEMENTED\n SHADER: %s", shaderPath);

	log_printf(log_level_e::LOG_DEBUG, "\nLoading Shader: %s", shaderPath);

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string shaderCode;
	std::ifstream shaderFile;

	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		// read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		// close file handlers
		shaderFile.close();

		// convert stream into string
		shaderCode = shaderStream.str();

		//vertex Shader
		size_t vertOffset = shaderCode.find("//#Begin_vert");
		size_t vertEndOffset = shaderCode.find("//#End_vert");
		size_t vertLenght = vertEndOffset - vertOffset;

		if (vertOffset != std::string::npos && vertEndOffset == std::string::npos)
			log_error("ERROR::SHADER::VERTEX_SHADER: //#End_vert not found");
		else
			vertexCode = shaderCode.substr(vertOffset, vertLenght);

		//Fragment shader
		size_t fragOffset = shaderCode.find("//#Begin_frag");
		size_t fragEndOffset = shaderCode.find("//#End_frag");
		size_t fragLenght = fragEndOffset - fragOffset;

		if (fragOffset != std::string::npos && fragEndOffset == std::string::npos) {
			log_error("ERROR::SHADER::FRAGMENT_SHADER: //#End_frag not found");
			errorOnLoad = true;
		}
		else
			fragmentCode = shaderCode.substr(fragOffset, fragLenght);
	}
	catch (std::ifstream::failure& e)
	{
		perror("	ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
		errorOnLoad = true;
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
	log_printf(log_level_e::LOG_DEBUG, "Shader Program ID: %i\n", m_ID);
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);
	checkCompileErrors(m_ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
