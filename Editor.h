#pragma once

#ifndef EDITOR_H
#define EDITOR_H

#include "Entity.h"
#include "Scene.h"
#include "IEditorGUISelectable.h"

namespace Engine
{
	class Editor
	{
	public:
		//Methods-------------------------------------
		static Editor* Instance() {
			static Editor* instance;

			if (!instance)
				instance = new Editor();

			return instance;
		}

		void Init();

		void renderGui(float deltaTime, Entity* root, Scene* scene);
		void renderHierarchyGui(Entity* entity);
		void renderEntityEditorPropertiesGui(Entity* entity);
		void renderAssetsGui();

		static Mesh* GUI_PropertySelector(const char* propName, Mesh* currentValue);
		static Material* GUI_PropertySelector(const char* propName, Material* currentValue);
		static Shader* GUI_PropertySelector(const char* propName, Shader* currentValue);
		static Texture* GUI_PropertySelector(const char* propName, Texture* currentValue);

		//Variables-----------------------------------
		bool showImGui_Demo = false;
		float dirLight_rotation_X = 90;

		IEditorGUISelectable* selected;

	private:
		//Variables-----------------------------------
		
		//Methods-------------------------------------
		Editor() {}
		Editor(Editor const&) = delete;
		Editor& operator=(Editor const&) = delete;
		~Editor() {};

	};
}

#endif
