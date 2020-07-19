#pragma once

#ifndef EngineIO_H
#define EngineIO_H

#include <filesystem>
namespace fs = std::filesystem;
#include <vector>
#include <string>

namespace Engine
{
	//Important: Don't try to use IO in the constructor of a global object may result in an error or unexpected behavoir
	namespace IO
	{

		const char* getProjectPath();
		void setProjectPath(const char* path);

		std::vector<std::string>* getFilePaths();

		std::vector<std::pair<std::string, std::string>>* getShaderPaths();

		std::vector<std::pair<std::string, std::string>>* getTexturesPaths();

		std::vector<std::pair<std::string, std::string>>* getModelPaths();

		void printProjectFiles();
		void scanFiles();
	}
}

#endif

