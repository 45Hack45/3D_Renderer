#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"

namespace Engine
{
	class Transform : public Component
	{
	public:
		//Methods-------------------------------------
		glm::mat4 localSpace()const;
		glm::mat4 globalSpace()const;
		glm::vec3 globalPosition()const;

		glm::mat4 operator*(const Transform& b)const {
			return localSpace() * b.localSpace();
		}

		void drawEditorGUI_Properties();

		//Variables-----------------------------------
		float meshScaleFactor = 1.f;
		Transform* parent;

		char* entityName;

		//Local transform info
		glm::vec3 m_position;
		glm::vec3 m_eulerAngle;//TODO: Change euler angles t quaternions
		glm::vec3 m_scale = glm::vec3(1.f);

	private:
		//Variables-----------------------------------

		//Methods-------------------------------------


	};
}

#endif
