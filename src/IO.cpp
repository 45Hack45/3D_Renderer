#include "IO.h"
#include <iostream>

namespace Engine
{
	//Important: Don't try to use IO in the constructor of a global object may result in an error or unexpected behavoir
	namespace IO {
		
		const char* projectPath;

		std::vector<std::string> filePaths;//contains all the project folder files and directories

		//pair<Name,Path>
		std::vector<std::pair<std::string, std::string>> shaderPaths;//contains all the project folder shaders
		std::vector<std::pair<std::string, std::string>> computeShaderPaths;
		std::vector<std::pair<std::string, std::string>> texturePaths;
		std::vector<std::pair<std::string, std::string>> modelPaths;

		const char* getProjectPath() { return projectPath; }
		void setProjectPath(const char* path) { projectPath = path; }

		std::vector<std::string>* getFilePaths() { return &filePaths; }

		std::vector<std::pair<std::string, std::string>>* getShaderPaths() { return &shaderPaths; }
		std::vector<std::pair<std::string, std::string>>* getComputeShaderPaths() { return &computeShaderPaths; } 

		std::vector<std::pair<std::string, std::string>>* getTexturesPaths() { return &texturePaths; }

		std::vector<std::pair<std::string, std::string>>* getModelPaths() { return &modelPaths; }

		void printProjectFiles() {
			for (auto& p : fs::recursive_directory_iterator(projectPath)) {
				//std::cout << p.path() << '\n';

				std::string s = p.path().string();

				std::cout << s << "		Has extension " << p.path().has_extension() << '\n';
			}
		}

		void printEngineRCSFiles() {
			for (auto& p : fs::recursive_directory_iterator("./rcs")) {
				//std::cout << p.path() << '\n';

				std::string s = p.path().string();

				std::cout << s << "		Has extension " << p.path().has_extension() << '\n';
			}
		}

		void scanFiles() {

			filePaths.clear();
			shaderPaths.clear();
			computeShaderPaths.clear();
			texturePaths.clear();
			modelPaths.clear();

			std::cout << "Active directory: " << fs::current_path() << std::endl;
			std::cout << "Scanning files..." << std::endl;
			//Scan Engine resources Path
			std::filesystem::recursive_directory_iterator rcs_directory_iterator;
			try{
				rcs_directory_iterator = fs::recursive_directory_iterator("./rcs");
			}
			catch (const std::exception& e) {
				std::cout << "Error Scanning Engine RCS: " << e.what() << std::endl;
				throw e;
			}
			std::cout << "Scanning Engine RCS..." << std::endl;
			for (auto& p : rcs_directory_iterator) {
				std::string path = p.path().string();
				// std::cout << path << '\n';
				std::string extension = p.path().extension().string();
				std::string nameExt = p.path().filename().string();//name with extension
				std::string name = nameExt.substr(0, nameExt.size() - extension.size());//name without extension

				//Convert extension to lowercase
				char c;
				int i = 0;
				std::string lExtension = "";
				while (extension[i])
				{
					lExtension += std::tolower(extension[i]);
					i++;
				}

				if (p.path().has_extension())
				{
					if (lExtension == ".glsl")
						shaderPaths.push_back(std::pair<std::string, std::string>(name, path));
					if (lExtension == ".comp")
						computeShaderPaths.push_back(std::pair<std::string, std::string>(name, path));

					else if (lExtension == ".jpg" || lExtension == ".png")
						texturePaths.push_back(std::pair<std::string, std::string>(name, path));

					else if (lExtension == ".obj" || lExtension == ".gltf")
						modelPaths.push_back(std::pair<std::string, std::string>(name, path));
				}

				filePaths.push_back(path);//Add path
			}

			//Scan Project Path
			std::filesystem::recursive_directory_iterator project_directory_iterator;
			try{
				project_directory_iterator = fs::recursive_directory_iterator(projectPath);
			}
			catch (const std::exception& e) {
				std::cout << "Error Scanning Project Files: " << e.what() << std::endl;
				throw e;
			}
			std::cout << "Scanning Project Files..." << std::endl;
			for (auto& p : project_directory_iterator) {
				std::string path = p.path().string();
				// std::cout << path << '\n';
				std::string extension = p.path().extension().string();
				std::string nameExt = p.path().filename().string();//name with extension
				std::string name = nameExt.substr(0, nameExt.size() - extension.size());//name without extension

				//Convert extension to lowercase
				char c;
				int i = 0;
				std::string lExtension = "";
				while (extension[i])
				{
					lExtension += std::tolower(extension[i]);
					i++;
				}

				if (p.path().has_extension())
				{
					if (lExtension == ".glsl")
						shaderPaths.push_back(std::pair<std::string, std::string>(name, path));
					if (lExtension == ".comp")
						computeShaderPaths.push_back(std::pair<std::string, std::string>(path, name));

					else if (lExtension == ".jpg" || lExtension == ".png")
						texturePaths.push_back(std::pair<std::string, std::string>(name, path));

					else if (lExtension == ".obj" || lExtension == ".gltf")
						modelPaths.push_back(std::pair<std::string, std::string>(name, path));
				}

				filePaths.push_back(path);//Add path
			}
		}
	}
}