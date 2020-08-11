#include "Asset.h"
#include "Editor.h"

namespace Engine
{
	void Engine::Asset::drawEditorGUI_Properties()
	{
		char* tName = new char[DisplayedNameMaxSize];

		tName = (char*)assetName.c_str();

		ImGui::InputText("Asset", tName, DisplayedNameMaxSize);
		assetName = tName;
	}
}
