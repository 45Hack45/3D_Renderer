#include "Editor.h"
#include "Engine.h"

#include <string>

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_glfw.h"
#include "imgui/examples/imgui_impl_opengl3.h"

#include "TextureManager.hpp"
#include "MaterialManager.hpp"

#define _nameMaxSize 64

#define _MaterialIcon "Engine_AssetGUI_Material_Icon_small"
#define _TextureIcon "Engine_AssetGUI_Texture_Icon_small"
#define _ModelIcon "Engine_AssetGUI_Model_Icon_small"
#define _MeshIcon "Engine_AssetGUI_Mesh_Icon_small"
#define _ShaderIcon "Engine_AssetGUI_Shader_Icon_small"

#define _DragDropMaterial "Material_DragDrop"
#define _DragDropTexture "Texture_DragDrop"
#define _DragDropMesh "Mesh_DragDrop"
#define _DragDropModel "Model_DragDrop"
#define _DragDropShader "Shader_DragDrop"

namespace Engine
{
	Engine* engine;

	void Editor::Init(){
		engine = Engine::Instance();
	}

	void Editor::renderHierarchyGui(Entity* entity) {

		std::string name = entity->name;

		std::string itemid = "##";
		itemid += entity->name;
		itemid += "_GUI_Selectable";

		ImGuiTreeNodeFlags flags = 0;

		if (entity->m_childs.size() <= 0)
			flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;

		if (entity == selected)
			flags |= ImGuiTreeNodeFlags_Selected;

		flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		bool open = ImGui::TreeNodeEx(name.c_str(), flags);
		bool clicked = ImGui::IsItemClicked();

		if (clicked && !ImGui::IsItemToggledOpen()) {
			selected = entity;
		}


		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HierarchyGUI_DragDrop")) {
				IM_ASSERT(payload->DataSize == sizeof(Entity*));
				Entity* p = *(Entity**)payload->Data;

				p->setParent(entity);
			}

			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_DragDropModel)) {
				IM_ASSERT(payload->DataSize == sizeof(Model*));
				Model* p = *(Model**)payload->Data;

				engine->scene->loadModel2Scene(p);
			}

			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource()) {
			
			ImGui::SetDragDropPayload("HierarchyGUI_DragDrop", &entity, sizeof(Entity**));
			ImGui::Text(entity->name.c_str());
			ImGui::EndDragDropSource();
		}

		//if (ImGui::IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Left)) {
		//	std::string temp = "##PlaceHolder_";
		//	temp += entity->name;
		//	ImGui::Selectable(temp.c_str(), false, 0, ImVec2(0, 5));
		//	if (ImGui::BeginDragDropTarget()) {
		//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HierarchyGUI_DragDrop")) {

		//		}
		//		ImGui::EndDragDropTarget();
		//	}
		//}

		if (open) {
			for (size_t i = 0; i < entity->m_childs.size(); i++)
			{
				renderHierarchyGui(entity->m_childs[i]);
			}

			ImGui::TreePop();
		}
	}

	void Editor::renderAssetsGui() {

		Texture* materialIcon = TextureManager::Instance()->getTexture(_MaterialIcon, false);
		Texture* TextureIcon = TextureManager::Instance()->getTexture(_TextureIcon, false);
		Texture* ModelIcon = TextureManager::Instance()->getTexture(_ModelIcon, false);
		Texture* MeshIcon = TextureManager::Instance()->getTexture(_MeshIcon, false);
		Texture* ShaderIcon = TextureManager::Instance()->getTexture(_ShaderIcon, false);

		if (ImGui::BeginTabBar("ResourcesTabBar")) {

			if (ImGui::BeginTabItem("Textures")) {

				auto mIterator = TextureManager::Instance()->BeginP();
				auto mIteratorEnd = TextureManager::Instance()->EndP();
				int i = 0;
				while (mIterator != mIteratorEnd)
				{

					Texture* value = TextureManager::Instance()->getTexture((*mIterator).first);//this will force to always load all textures

					if (!value) {
						mIterator++;
						i++;
						continue;
					}
					if (value->filePath.find("#HIDE#") != std::string::npos)//If filePath contains #HIDE# do not list in the GUI
					{
						mIterator++;
						i++;
						continue;
					}

					std::string itemid = "##" + std::to_string(i);
					if (ImGui::Selectable(itemid.c_str(), false, 0, ImVec2(0, 32))) {
						selected = value;
					}

					if (ImGui::BeginDragDropSource()) {

						ImGui::SetDragDropPayload(_DragDropTexture, &value, sizeof(Texture**));
						ImGui::Text(value->assetName.c_str());
						ImGui::EndDragDropSource();
					}

					ImGui::SameLine();
					ImGui::Image((void*)(intptr_t)TextureIcon->ID(), ImVec2(32, 32));
					ImGui::SameLine();
					ImGui::Text(value->assetName.c_str());

					mIterator++;
					i++;
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Materials")) {

				auto mIterator = MaterialManager::Instance()->Begin();
				auto mIteratorEnd = MaterialManager::Instance()->End();
				int i = 0;
				while (mIterator != mIteratorEnd)
				{

					Material* value = (*mIterator).second;

					std::string itemid = "##" + std::to_string(i);
					if (ImGui::Selectable(itemid.c_str(),false,0,ImVec2(0,32))) {
						selected = value;
					}

					if (ImGui::BeginDragDropSource()) {

						ImGui::SetDragDropPayload(_DragDropMaterial, &value, sizeof(Material**));
						ImGui::Text(value->name.c_str());
						ImGui::EndDragDropSource();
					}

					ImGui::SameLine();
					ImGui::Image((void*)(intptr_t)materialIcon->ID(), ImVec2(32, 32));
					ImGui::SameLine();
					ImGui::Text(value->name.c_str());

					mIterator++;
					i++;
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Models")) {

				auto mIterator = ModelManager::Instance()->BeginP();
				auto mIteratorEnd = ModelManager::Instance()->EndP();
				int i = 0;
				while (mIterator != mIteratorEnd)
				{

					Model* value = ModelManager::Instance()->getModel((*mIterator).first);//this will force to always load all shaders

					if (!value) {
						mIterator++;
						i++;
						continue;
					}
					if (value->filePath.find("#HIDE#") != std::string::npos)//If filePath contains #HIDE# do not list in the GUI
					{
						mIterator++;
						i++;
						continue;
					}

					//std::string itemid = "##" + std::to_string(i);
					//if (ImGui::Selectable(itemid.c_str(), false, 0, ImVec2(0, 32))) {
					//	selected = value;
					//}

					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

					if (selected == value)
						flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;

					bool opened = ImGui::TreeNodeEx(value->assetName.c_str());
					bool clicked = ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left);
					bool togleOpen = ImGui::IsItemToggledOpen();

					if (ImGui::BeginDragDropSource()) {

						ImGui::SetDragDropPayload(_DragDropModel, &value, sizeof(Model**));
						ImGui::Text(value->assetName.c_str());
						ImGui::EndDragDropSource();
					}

					//ImGui::SameLine();
					//ImGui::Image((void*)(intptr_t)ModelIcon->ID(), ImVec2(32, 32));
					//ImGui::SameLine();
					//ImGui::Text(value->assetName.c_str());

					if (clicked && togleOpen)
						selected = value;

					if (opened) {

						for (size_t i = 0; i < value->m_numMeshes; i++){
							std::string meshid = "##" + std::to_string(i);
							ImGui::Selectable(meshid.c_str(), false, 0, ImVec2(0, 32));

							if (ImGui::BeginDragDropSource()) {
								ImGui::SetDragDropPayload(_DragDropMesh, &value, sizeof(Mesh**));
								ImGui::Text(value->m_meshes[i].name);
								ImGui::EndDragDropSource();
							}

							ImGui::SameLine();
							ImGui::Image((void*)(intptr_t)MeshIcon->ID(), ImVec2(32, 32));
							ImGui::SameLine();
							ImGui::Text(value->m_meshes[i].name);
						}

						ImGui::TreePop();
					}

					mIterator++;
					i++;
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Shaders")) {

				auto mIterator = ShaderManager::Instance()->BeginP();
				auto mIteratorEnd = ShaderManager::Instance()->EndP();
				int i = 0;
				while (mIterator != mIteratorEnd)
				{

					Shader* value = ShaderManager::Instance()->getShader((*mIterator).first);//this will force to always load all shaders

					if (!value) {
						mIterator++;
						i++;
						continue;
					}
					if (value->filePath.find("#HIDE#") != std::string::npos)//If filePath contains #HIDE# do not list in the GUI
					{
						mIterator++;
						i++;
						continue;
					}

					std::string itemid = "##" + std::to_string(i);
					if (ImGui::Selectable(itemid.c_str(), false, 0, ImVec2(0, 32))) {
						selected = value;
					}

					if (ImGui::BeginDragDropSource()) {

						ImGui::SetDragDropPayload(_DragDropShader, &value, sizeof(Shader**));
						ImGui::Text(value->assetName.c_str());
						ImGui::EndDragDropSource();
					}

					ImGui::SameLine();
					ImGui::Image((void*)(intptr_t)ShaderIcon->ID(), ImVec2(32, 32));
					ImGui::SameLine();
					ImGui::Text(value->assetName.c_str());

					mIterator++;
					i++;
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	void Editor::renderEntityEditorPropertiesGui(Entity* entity) {

		if (!entity)
			return;

		char* name = new char[_nameMaxSize];

		name = (char*)entity->name.c_str();

		ImGui::InputText("Entity", name, _nameMaxSize);
		entity->name = name;

		//ImGui::Text(entity->name.c_str());
		ImGui::Spacing();

		//Draw Transform
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			entity->transform.drawEditorGUI_Properties();
		}

		ImGui::Spacing();
		ImGui::Spacing();

		//Draw meshrenderer
		if (ImGui::CollapsingHeader("MeshRenderer"))
			entity->meshRenderer.drawEditorGUI_Properties();

		ImGui::Spacing();
		ImGui::Spacing();

		//Draw material
		if (entity->meshRenderer.m_material)
			if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
				entity->meshRenderer.m_material->drawEditorGUI_Properties();

		ImGui::Spacing();
		ImGui::Spacing();

		//Draw components
		for (auto component : entity->components) {
			if (ImGui::CollapsingHeader(component.name.c_str()))
			{
				component.drawEditorGUI_Properties();
			}
			ImGui::Spacing();
		}
	}

	void Editor::renderGui(float deltaTime, Entity* root, Scene* scene)
	{
		//-----------------------------Render GUI------------------------------------------

		//Main Menu
		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("File"))
			{

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Entity"))
			{
				if (ImGui::Button("Create Entity")) {
					scene->createEntity();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}


		ImGui::Begin("Frame Rate", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", deltaTime * 1000, 1 / (deltaTime));
		ImGui::End();

		ImGuiWindowFlags windowsFlags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		//Resources
		if (ImGui::Begin("Resources", NULL, windowsFlags)) {
			renderAssetsGui();
		}
		ImGui::End();

		//hierarchy
		if (ImGui::Begin("Scene", NULL, windowsFlags)) {
			//ImVec2 size = ImGui::GetWindowSize();
			////Dummy
			//ImGui::SameLine(); 
			//ImGui::Dummy(ImVec2(0,0));

			//if (ImGui::BeginDragDropTarget()) {
			//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_DragDropModel)) {
			//		IM_ASSERT(payload->DataSize == sizeof(Model*));
			//		Model* m = *(Model**)payload->Data;
			//		scene->loadModel2Scene(m);
			//	}

			//	ImGui::EndDragDropTarget();
			//}

			//ImGui::SameLine();
			
			//renderHierarchyGui(&scene->m_scene);

			for (auto entity : root->m_childs)
				renderHierarchyGui(entity);
		}
		ImGui::End();

		//Properties
		if (ImGui::Begin("Properties", NULL, windowsFlags)) {
			if(selected)
				selected->drawEditorGUI_Properties();
		}
		ImGui::End();

		//Light Window
		if (ImGui::Begin("Lights", NULL, windowsFlags)) {
			//Directional light
			if (ImGui::TreeNode("Directional Light")) {

				glm::vec3 dir = scene->m_directionalLight.getDirection();

				ImGui::Text("Direction: (%f,%f,%f)", dir.x, dir.y, dir.z);

				ImGui::ColorEdit3("Color", (float*)&(scene->m_directionalLight.m_color.color_vect));
				//ImGui::SliderAngle("X Rotation", &dirLight_rotation_X);
				ImGui::DragFloat("Yaw", (float*)&scene->m_directionalLight.yaw);
				ImGui::DragFloat("Pitch", (float*)&scene->m_directionalLight.pitch);
				scene->m_directionalLight.updateDirection();

				ImGui::SliderFloat("Intensity", &scene->m_directionalLight.m_intensity, 0, 5);

				ImGui::TreePop();
			}

			//Point Lights
			if (ImGui::TreeNode("Point Lights")) {
				if (scene->m_pointLights.size() < 10)
					if (ImGui::Button("Create New Point Light"))
					{
						LightSource_Point l(Color::White, 1.f, glm::vec3(0.0f));
						l.m_linear = .35f;
						l.m_quadratic = .44f;
						scene->m_pointLights.push_back(l);
					}

				for (int i = 0; i < scene->m_pointLights.size(); i++)
				{
					std::string lightNode = "Light_" + std::to_string(i);
					if (ImGui::TreeNode(lightNode.c_str()))
					{
						ImGui::ColorEdit3("Color", (float*)&scene->m_pointLights[i].m_color.color_vect);
						ImGui::DragFloat3("Position", (float*)&scene->m_pointLights[i].m_pos);

						ImGui::Spacing();

						ImGui::SliderFloat("Intensity", &(scene->m_pointLights[i].m_intensity), 0, 5);
						ImGui::SliderFloat("Linear", &(scene->m_pointLights[i].m_linear), 0, .5);
						ImGui::SliderFloat("Quadratic", &(scene->m_pointLights[i].m_quadratic), 0, .1);
						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}

			//Spot Lights
			if (ImGui::TreeNode("Spot Lights")) {
				if (scene->m_spotLights.size() < 10)
					if (ImGui::Button("Create New Spot Light"))
					{
						LightSource_Spot l(Color::White, 1.f, glm::vec3(0, 10, 0), glm::vec3(0, -1, 0), 5, 5);
						l.m_linear = .35f;
						l.m_quadratic = .44f;
						scene->m_spotLights.push_back(l);
					}

				for (int i = 0; i < scene->m_spotLights.size(); i++)
				{
					std::string lightNode = "Light_" + std::to_string(i);
					if (ImGui::TreeNode(lightNode.c_str()))
					{
						ImGui::ColorEdit3("Color", (float*)&scene->m_spotLights[i].m_color.color_vect);

						ImGui::Spacing();

						ImGui::DragFloat3("Position", (float*)&scene->m_spotLights[i].m_pos);
						ImGui::SliderFloat3("Direction", (float*)&scene->m_spotLights[i].m_dir, -90, 90);

						ImGui::Spacing();

						ImGui::SliderFloat("Intensity", &(scene->m_spotLights[i].m_intensity), 0, 5);

						ImGui::Spacing();

						ImGui::Spacing(); ImGui::SliderFloat("Inner Angle", &(scene->m_spotLights[i].m_angle), 0, 45);
						//if (scene->m_spotLights[i].m_outerAngle < scene->m_spotLights[i].m_angle)
						//	scene->m_spotLights[i].m_outerAngle = scene->m_spotLights[i].m_angle;

						ImGui::Spacing(); ImGui::SliderFloat("Outer Angle", &(scene->m_spotLights[i].m_outerAngle), 0, 45);

						ImGui::Spacing();

						ImGui::SliderFloat("Linear", &(scene->m_spotLights[i].m_linear), 0, .5);
						ImGui::SliderFloat("Quadratic", &(scene->m_spotLights[i].m_quadratic), 0, .1);

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
		}
		ImGui::End();

		if (showImGui_Demo)
			ImGui::ShowDemoWindow(&showImGui_Demo);

		
	}

	Mesh* Editor::GUI_PropertySelector(const char* propName, Mesh* currentValue)
	{
		Mesh* selectedMesh = currentValue;

		std::string name = "NONE";

		if (currentValue)
			name = currentValue->name;

		bool opened = ImGui::BeginCombo(propName, name.c_str());

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_DragDropMesh)) {
				if (payload->DataSize != sizeof(Mesh**))
					log_error("ERROR::EDITOR::GUI_PROPERTY_SELECTOR::FAILED_DROP_OPERATION");
				Mesh* p = *(Mesh**)payload->Data;

				//Pre exiting

				ImGui::EndDragDropTarget();

				if (opened)//In this case should never be true in general.
					ImGui::EndCombo();

				return p;
			}

			ImGui::EndDragDropTarget();
		}

		if (opened) {

			auto modelsIterator = ModelManager::Instance()->Begin();
			auto modelsIteratorEnd = ModelManager::Instance()->End();

			while (modelsIterator != modelsIteratorEnd)
			{
				for (size_t i = 0; i < (*modelsIterator).second->m_numMeshes; i++)
				{
					Mesh* mesh = &(*modelsIterator).second->m_meshes[i];
					bool selected = mesh == currentValue;

					if (ImGui::Selectable(mesh->name, selected))
						selectedMesh = mesh;
					if (selected)
						ImGui::SetItemDefaultFocus();
				}

				modelsIterator++;
			}

			ImGui::EndCombo();
		}

		return selectedMesh;
	}

	Material* Editor::GUI_PropertySelector(const char* propName, Material* currentValue)
	{
		Material* selectedValue = currentValue;

		std::string name = "NONE";

		if (currentValue)
			name = currentValue->name;

		bool opened = ImGui::BeginCombo(propName, name.c_str());

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_DragDropMaterial)) {
				if (payload->DataSize != sizeof(Material**))
					log_error("ERROR::EDITOR::GUI_PROPERTY_SELECTOR::FAILED_DROP_OPERATION");

				Material* p = *(Material**)payload->Data;

				//Pre exiting

				ImGui::EndDragDropTarget();

				if (opened)//In this case should never be true in general.
					ImGui::EndCombo();

				return p;
			}

			ImGui::EndDragDropTarget();
		}

		if (opened) {

			auto mIterator = MaterialManager::Instance()->Begin();
			auto mIteratorEnd = MaterialManager::Instance()->End();

			while (mIterator != mIteratorEnd)
			{

				Material* value = (*mIterator).second;
				bool selected = value == currentValue;

				if (ImGui::Selectable(value->name.c_str(), selected))
					selectedValue = value;
				if (selected)
					ImGui::SetItemDefaultFocus();

				mIterator++;
			}

			ImGui::EndCombo();
		}

		return selectedValue;
	}

	Shader* Editor::GUI_PropertySelector(const char* propName, Shader* currentValue)
	{
		Shader* selectedValue = currentValue;

		std::string name = "NONE";

		if (currentValue)
			name = currentValue->assetName;

		bool opened = ImGui::BeginCombo(propName, name.c_str());

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_DragDropShader)) {
				if (payload->DataSize != sizeof(Shader**))
					log_error("ERROR::EDITOR::GUI_PROPERTY_SELECTOR::FAILED_DROP_OPERATION");
				Shader* p = *(Shader**)payload->Data;

				//Pre exiting

				ImGui::EndDragDropTarget();

				if (opened)//In this case should never be true in general.
					ImGui::EndCombo();

				return p;
			}

			ImGui::EndDragDropTarget();
		}

		if (opened) {

			auto mIterator = ShaderManager::Instance()->Begin();
			auto mIteratorEnd = ShaderManager::Instance()->End();

			while (mIterator != mIteratorEnd)
			{

				Shader* value = (*mIterator).second;
				bool selected = value == currentValue;

				if (ImGui::Selectable(value->assetName.c_str(), selected))
					selectedValue = value;
				if (selected)
					ImGui::SetItemDefaultFocus();

				mIterator++;
			}

			ImGui::EndCombo();
		}

		return selectedValue;
	}

	Texture* Editor::GUI_PropertySelector(const char* propName, Texture* currentValue)
	{
		Texture* selectedValue = currentValue;

		std::string name = "NONE";

		if (currentValue)
			name = currentValue->assetName.c_str();

		bool opened = ImGui::BeginCombo(propName, name.c_str());

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_DragDropTexture)) {
				if (payload->DataSize != sizeof(Texture**))
					log_error("ERROR::EDITOR::GUI_PROPERTY_SELECTOR::FAILED_DROP_OPERATION");
				Texture* p = *(Texture**)payload->Data;

				//Pre exiting

				ImGui::EndDragDropTarget();

				if(opened)//In this case should never be true in general.
					ImGui::EndCombo();

				return p;
			}

			ImGui::EndDragDropTarget();
		}

		if (opened) {

			auto mIterator = TextureManager::Instance()->Begin();
			auto mIteratorEnd = TextureManager::Instance()->End();

			while (mIterator != mIteratorEnd)
			{

				Texture* value = (*mIterator).second;
				bool selected = value == currentValue;

				if (ImGui::Selectable(value->assetName.c_str(), selected))
					selectedValue = value;
				if (selected)
					ImGui::SetItemDefaultFocus();

				mIterator++;
			}

			ImGui::EndCombo();
		}

		return selectedValue;
	}
}
