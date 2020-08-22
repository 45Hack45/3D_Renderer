#include "Shader.h"
#include <stdio.h>

#define _vertBegin	"//#Begin_vert"
#define _vertEnd	"//#End_vert"
#define _fragBegin	"//#Begin_frag"
#define _fragEnd	"//#End_frag"
#define _geoBegin	"//#Begin_geo"
#define _geogEnd	"//#End_geo"
#define _propBegin	"//#Begin_prop"
#define _propEnd	"//#End_prop"


Shader::Shader(const char* vertexShader, const char* fragmentShader, bool isFile) : Asset(vertexShader, vertexShader)
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

		getProperties(vertexCode);
		getProperties(fragmentCode);
	}
	else {
		vShaderCode = vertexShader;
		fShaderCode = fragmentShader;

		getProperties(vertexShader);
		getProperties(fragmentShader);
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

void Shader::loadFile() {
	//log_printf(log_level_e::LOG_INFO, "ERROR::SHADER::CONSTRUCTOR_NOT_IMPLEMENTED\n SHADER: %s", shaderPath);

	log_printf(log_level_e::LOG_DEBUG, "\nLoading Shader: %s", filePath.c_str());

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode; bool hasGeometryCode = false;
	std::string shaderCode;
	std::ifstream shaderFile;

	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		shaderFile.open(filePath);
		std::stringstream shaderStream;

		// read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		// close file handlers
		shaderFile.close();

		// convert stream into string
		shaderCode = shaderStream.str();

		//vertex Shader
		size_t vertOffset = shaderCode.find(_vertBegin);
		size_t vertEndOffset = shaderCode.find(_vertEnd);
		size_t vertLenght = vertEndOffset - vertOffset;

		if (vertOffset != std::string::npos)
		{
			if (vertEndOffset == std::string::npos) {
				log_error("ERROR::SHADER::VERTEX_SHADER: //#End_vert not found");
				errorOnLoad = true;
			}
			else
				vertexCode = shaderCode.substr(vertOffset, vertLenght);
		}
		else {
			log_error("ERROR::SHADER::VERTEX_SHADER: Vertex code not found");
			errorOnLoad = true;
		}

		//Fragment shader
		size_t fragOffset = shaderCode.find(_fragBegin);
		size_t fragEndOffset = shaderCode.find(_fragEnd);
		size_t fragLenght = fragEndOffset - fragOffset;

		if (fragOffset != std::string::npos) {
			if (fragEndOffset == std::string::npos) {
				log_error("ERROR::SHADER::FRAGMENT_SHADER: //#End_frag not found");
				errorOnLoad = true;
			}else
				fragmentCode = shaderCode.substr(fragOffset, fragLenght);
		}
		else {
			log_error("ERROR::SHADER::FRAGMENT_SHADER: Fragment code not found");
			errorOnLoad = true;
		}
			

		//geometry Shader(optional)
		size_t geoOffset = shaderCode.find(_geoBegin);
		size_t geoEndOffset = shaderCode.find(_geogEnd);
		size_t geoLenght = geoEndOffset - geoOffset;

		size_t p = std::string::npos;

		if(geoOffset != std::string::npos)
			if (geoEndOffset == std::string::npos) {
				hasGeometryCode = false;
				log_error("ERROR::SHADER::GEOMETRY_SHADER: //#End_geo not found");
				errorOnLoad = true;
			}
			else {
				hasGeometryCode = true;
				geometryCode = shaderCode.substr(geoOffset, geoLenght);
			}
		else
			hasGeometryCode = false;
		
	}
	catch (std::ifstream::failure& e)
	{
		perror("	ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
		errorOnLoad = true;
		return;
	}

	if (errorOnLoad)return;

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	getProperties(shaderCode);


	// 2. compile shaders
	unsigned int vertex = 0, fragment = 0, geometry = 0;

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


	// geometry Shader (optional)
	if (hasGeometryCode) {
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}


	// shader Program
	m_ID = glCreateProgram();
	log_printf(log_level_e::LOG_DEBUG, "	Shader Program ID: %i\n", m_ID);
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	if (hasGeometryCode) glAttachShader(m_ID, geometry);

	// 3. link shaders program
	glLinkProgram(m_ID);
	checkCompileErrors(m_ID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (hasGeometryCode)glDeleteShader(geometry);
}

void Shader::getProperties(const std::string& shaderCode) {
	
	size_t propBegin, propEnd;

	propBegin = shaderCode.find(_propBegin);

	if (propBegin == shaderCode.npos)
	{//Propeties not found
		log_message(log_level_e::LOG_DEBUG, "	Shader Properties not found.");
		hasPropeties = false;
		return;
	}

	propEnd = shaderCode.find(_propEnd);

	if (propEnd == shaderCode.npos)
	{
		log_error("ERROR::SHADER::PROPERTIES: //#End_prop not found");
		hasPropeties = false;
		return;
	}

	propBegin += sizeof(_propBegin);//offseting to end of _propBegin to avoid including _propBegin in the propertiesCode

	std::string propertiesCode = shaderCode.substr(propBegin, propEnd - propBegin);
	std::stringstream ss(propertiesCode);
	std::string to;

	if (propertiesCode.size() > 0)
	{
		while (std::getline(ss, to, '\n')) {

			shaderProperties.push_back(to);
		}
		hasPropeties = true;
	}
	else {
		hasPropeties = false;
	}
	
}

