#pragma once

#ifndef Material_MANAGER_H_
#define Material_MANAGER_H_

#include "Material.h"
#include <string>

namespace Engine
{
	class MaterialManager
	{
	public:
		//Methods-------------------------------------
		static MaterialManager* Instance() {
			static MaterialManager* instance;

			if (!instance)
				instance = new MaterialManager();

			return instance;
		}

		void AddMaterial(Material* material) {
			materials[material->name] = material;
		}

		Material* getMaterial(const std::string& materialName) {
			if (materials.find(materialName) == materials.end())
				return nullptr;

			return materials[materialName];
		}

		auto Begin() {
			return materials.begin();
		}
		auto End() {
			return materials.end();
		}

		//Variables-----------------------------------

	private:

		//Methods-------------------------------------
		MaterialManager() {

		}
		MaterialManager(MaterialManager const&) = delete;
		MaterialManager& operator=(MaterialManager const&) = delete;
		~MaterialManager() {};


		//Variables-----------------------------------
		std::unordered_map<std::string, Material*> materials;
	};
}

#endif
