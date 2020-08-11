#include "MeshRenderer.h"
#include "Editor.h"

namespace Engine
{

	//TODO: can change material and mesh using the displayed GUI
	void MeshRenderer::drawEditorGUI_Properties()
	{
		m_mesh = Editor::GUI_PropertySelector("Mesh",m_mesh);
		m_material = Editor::GUI_PropertySelector("material", m_material);
	}

}
