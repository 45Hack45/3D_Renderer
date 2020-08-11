#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "Component.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include <string>

#include "IEditorGUISelectable.h"

namespace Engine
{
	class Entity: public IEditorGUISelectable
	{

	public:
		//Methods-------------------------------------

		Entity() {
			name = "Entity";
			m_parent = NULL;
			transform.entityName = name.data();
		}
		Entity(Entity* parent){
			name = "Entity";
			m_parent = parent;
			transform.parent = &parent->transform;
			transform.entityName = name.data();
		}

		void drawEditorGUI_Properties();

		Entity*	getParent() { return m_parent; }
		void	setParent(Entity* parent);

		bool isChild(Entity* entity);

		//Creates a new entity and sets it as child
		//Retruns a pointer to the child
		Entity*	addChild();
		void addChild(Entity* entity);

		//Variables-----------------------------------

		std::string name;
		Transform transform;

		MeshRenderer meshRenderer;

		Entity* m_parent;
		std::vector<Entity*> m_childs;
		std::vector<Component> components;

	private:
		//Variables-----------------------------------

		//Methods-------------------------------------
		Entity(Entity& entity) = delete;

		bool check_recursion(Entity* entity);
	};
}

#endif
