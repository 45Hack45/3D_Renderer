#pragma once

#ifndef SCENE_H
#define SCENE_H

#include "LightSource.h"
#include "Entity.h"
#include "Model.h"

namespace Engine
{
	class LightingInfo
	{
	public:
		LightSource_Directional* directional;
		std::vector<LightSource_Point>* pointLights;
		std::vector<LightSource_Spot>* spotLights;
	};

	class Scene
	{
	public:
		//Methods-------------------------------------
		Scene() : m_directionalLight(Color::White, 1, glm::vec3(0, 1, 0)) {
			m_directionalLight.m_intensity = 1;;
			m_pointLights.reserve(10);
			m_spotLights.reserve(10);

			m_scene.name = "Scene_Root";
		}

		//Creates an entity and returns a pointer to the new entity
		Entity* createEntity();
		Entity* createEntity(Entity* parent);
		Entity* loadModel2Scene(Model* model);

		//Variables-----------------------------------
		Entity m_scene;
		std::vector<Entity*> entities;

		LightSource_Directional m_directionalLight;
		std::vector<LightSource_Point> m_pointLights;
		std::vector<LightSource_Spot> m_spotLights;

	private:
		//Variables-----------------------------------

		//Methods-------------------------------------
		void loadModelMaterials(Model* model);

		void createModelEntity(Model* model, aiNode* parentNode, Entity* parent);
		Entity* createModelEntityMeshes(Model* model, aiNode* parentNode, Entity* parent);
	};
}

#endif
