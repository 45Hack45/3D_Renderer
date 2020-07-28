#pragma once

#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"
#include "Model.h"
#include "Material.h"

namespace Engine
{
	class MeshRenderer : Component
	{
	public:

		void Init(Mesh* mesh, Material* material) {
			m_mesh = mesh;
			m_material = material;
		}

		void setMesh(Mesh* mesh) { m_mesh = mesh; }
		void setMaterial(Material* material) { m_material = material; }

		void Render() {
			m_mesh->Draw();
		}

		Mesh* m_mesh;
		Material* m_material;

	private:


	};
}

#endif
