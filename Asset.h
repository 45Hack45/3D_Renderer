#pragma once

#ifndef ASSET_H
#define ASSET_H

#include "logger.h"
#include <string>
#include <functional>//required for the hash method

namespace Engine
{
	class Asset
	{
	public:

		Asset(const std::string& file, const std::string& name) {
			filePath = file;
			assetName = name;
		}

		std::string assetName;
		std::string filePath;

		virtual void loadFile() {
			log_error("ERROR::ASSET::CALLED_BASE_CLASS_METHOD:: void loadFile\n");
		}

		size_t hash() {
			size_t Hpath = std::hash<std::string>{}(filePath);
			size_t HName = std::hash<std::string>{}(assetName);

			return Hpath ^ (HName << 1);
		}

	protected:

		std::string fileDirectory() { 
			size_t separatorIndex = filePath.find_last_of('/');

			if (separatorIndex == std::string::npos) {
				log_printf(log_level_e::LOG_INFO, "ERROR::ASSET::FILE_DIRECTORY_NOT_FOUND:: %s", filePath);
				return "";
			}

			return filePath.substr(0, separatorIndex);
		
		}
		std::string fileName() {
			size_t separatorIndex = filePath.find_last_of('/')+1;
			size_t extensionIndex = filePath.find_last_of('.');

			if (separatorIndex == std::string::npos)
				separatorIndex = 0;

			if (extensionIndex == std::string::npos)
				extensionIndex = (size_t)filePath.size();

			return filePath.substr(separatorIndex, extensionIndex - separatorIndex);
		}
		std::string fileExtension() {
			size_t separatorIndex = filePath.find_last_of('.');

			if (separatorIndex == std::string::npos) {
				log_printf(log_level_e::LOG_INFO, "ERROR::ASSET::FILE_EXTENSION_NOT_FOUND:: %s", filePath);
				return "";
			}

			return filePath.substr(separatorIndex, filePath.size());
		}
		std::string fileNameAndExtension() {
			size_t separatorIndex = filePath.find_last_of('/')+1;

			if (separatorIndex == std::string::npos) {
				log_printf(log_level_e::LOG_INFO, "ERROR::ASSET::FILE_DIRECTORY_NOT_FOUND:: %s", filePath);
				return "";
			}

			return filePath.substr(separatorIndex, filePath.size()- separatorIndex);

		}
	};

}

#endif

