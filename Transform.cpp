#include "Transform.h"
#include "Editor.h"

namespace Engine
{
	glm::mat4 Transform::localSpace()const {
		glm::mat4 model = glm::mat4(1.f);

		model = glm::translate(model, m_position);

		model = glm::rotate(model, glm::radians(m_eulerAngle.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_eulerAngle.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_eulerAngle.z), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, m_scale * meshScaleFactor);

		return model;
	}
	glm::mat4 Transform::globalSpace() const
	{
		if (parent)
			return parent->globalSpace() * localSpace();
		else//is root
			return localSpace();
	}

	void Transform::drawEditorGUI_Properties()
	{
		ImGui::DragFloat3("Position", (float*)&m_position);
		ImGui::DragFloat3("EulerAngles", (float*)&m_eulerAngle);
		ImGui::DragFloat3("Scale", (float*)&m_scale);
		//ImGui::SameLine();
		ImGui::DragFloat("Scale Factor", &meshScaleFactor);
	}
}

