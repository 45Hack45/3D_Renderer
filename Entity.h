#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "Component.h"
#include "Transform.h"
#include "MeshRenderer.h"

namespace Engine
{
	class Entity
	{

	public:
		//Methods-------------------------------------

		Entity() {
			name = "Entity";
			m_parent = NULL;
		}
		Entity(Entity* parent){
			name = "Entity";
			m_parent = parent;
			transform.parent = &parent->transform;
		}

		Entity*	getParent() { return m_parent; }
		void	setParent(Entity* parent);

		//Creates a new entity and sets it as child
		//Retruns a pointer to the child
		Entity*	addChild();

		//Variables-----------------------------------

		const char* name;
		Transform transform;

		MeshRenderer meshRenderer;

	private:
		//Variables-----------------------------------
		Entity* m_parent;
		std::vector<Entity*> m_childs;

		std::vector<Component> components;

		//Methods-------------------------------------
		Entity(Entity& entity) = delete;
	};
}

#endif
