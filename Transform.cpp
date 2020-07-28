#include "Transform.h"

namespace Engine
{
	glm::mat4 Transform::localSpace()const {
		glm::mat4 model = glm::rotate(glm::mat4(1.f), glm::radians(m_eulerAngle.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_eulerAngle.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_eulerAngle.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, m_scale * meshScaleFactor);

		return glm::translate(model, m_position);
	}
	glm::mat4 Transform::globalSpace() const
	{
		if (parent)
			return parent->globalSpace() * localSpace();
		else//is root
			return localSpace();
	}
}

