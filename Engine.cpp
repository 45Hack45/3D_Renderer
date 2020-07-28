#include "Engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <stdlib.h>
#include <map>

#include "IO.h"
#include "Texture.h"
#include "Input.h"
#include "Material.h"


#define unlitShaderVert "./Shaders/UnlitShader.vert"
#define unlitShaderFrag "./Shaders/UnlitShader.frag"

#define phongShaderVert "./Shaders/PhongShader.vert"
#define phongShaderFrag "./Shaders/PhongShader.frag"

#define useUnlitShader false

#define ambientLight .06

namespace Engine
{
	void imgui_beginFrame() {
		//GUI window
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void imgui_RenderFrame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void initImgui(GLFWwindow* window, bool darkMode = true) {

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Setup Dear ImGui style
		if (darkMode)
			ImGui::StyleColorsDark();
		else
			ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Read 'docs/FONTS.txt' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);
	}

	int Engine::Init(const char* projectPath, const char* projectName) {
		log_message(log_level_e::LOG_INFO, "Initializing Engine");
		IO::setProjectPath(projectPath);

		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
			return -1;

		renderer = Renderer::Instance();

		//Init renderer
		int error = renderer->Init(projectName);

		//Init glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			log_error("ERROR::ENGINE:FAILED_TO_INIT_GLAD\n");
			return -1;
		}

		//Init imgui
		initImgui(renderer->m_window);

		input = Input::Input::Instance();
		input->Init(&renderer->m_window);

		IO::scanFiles();

		shaderManager = ShaderManager::Instance();
		shaderManager->Init(false);
		Shader* shader_phong = shaderManager->getShader("PhongShader");
		Shader* shader_unlit = shaderManager->getShader("UnlitShader");

		textureManager = TextureManager::Instance();
		textureManager->Init(false);

		modelManager = ModelManager::Instance();
		modelManager->Init(false);

		materialManager = MaterialManager::Instance();

		log_message(log_level_e::LOG_INFO, "Engine Initialized\n");
	}

	void Engine::Start() {
		//m_model = new Model("./rcs/sponza/Sponza.gltf", "Backpack");

		m_model = modelManager->getModel("Sponza",false);//Geting and loading model
		m_model->loadFile(.1f);

		//IO::printProjectFiles();
	}

	void Engine::mainLoop() {
		log_message(log_level_e::LOG_DEBUG, "\n\n\n--------------------------Starting Main Loop------------------------\n");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//log_message(log_level_e::LOG_DEBUG, "Loading Shader\n");

		//const char* vertPath = (useUnlitShader) ? unlitShaderVert : phongShaderVert;
		//const char* fragPath = (useUnlitShader) ? unlitShaderFrag : phongShaderFrag;

		//Shader shader(vertPath, fragPath);
		//Shader shader("./Shaders/PhongShader.glsl");
		
		//log_printf(log_level_e::LOG_INFO, "Shader error %d", shader.errorOnLoad);

		//shader_phong->use();

		//Camera
		Camera cam = Camera(glm::vec3(0, 1, 0));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(glm::rotate(model, glm::radians(-45.0f * 0), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.f, 1.f, 1.f)*1.f);

		/*float iniTime = glfwGetTime();
		//Texture diffuseText("./rcs/backpack/diffuse.jpg");
		//Texture aoText("./rcs/backpack/ao.jpg");
		//Texture normalText("./rcs/backpack/normal.png");
		//Texture roughnessText("./rcs/backpack/roughness.jpg");
		//Texture specularText("./rcs/backpack/specular.jpg");
		
		//Texture grassText("./rcs/Amaryllis City/OBJ/Amaryllis City/Grass.jpg");
		
		//Texture* wallTexture = textureManager->getTexture("diffuse");

		//wallTexture->bind();

		float finTime = glfwGetTime();

		std::cout << "Load Textures time: " << finTime - iniTime << std::endl << std::endl;*/

		//Material material1(shaderManager->getShader("PhongShader"));
		//material1.name = "Material_1";

		//material1.setTexture2D("albedoTexture", "diffuse");
		//material1.setFloat("shinines", 16);
		//material1.setFloat("specularIntensity", 1.f);
		//material1.setColor("myColor", Color::White);

		////material.printPropNames();
		//material1.sendMaterial2Shader();

		//Material material2(shaderManager->getShader("PhongShader"));
		//material2.name = "Material_2";

		//material2.setTexture2D("albedoTexture", "diffuse");
		//material2.setFloat("shinines", 255);
		//material2.setFloat("specularIntensity", 5.f);
		//material2.setColor("myColor", Color::White);

		////material2.printPropNames();
		//material2.sendMaterial2Shader();

		//Entity* entity0 = createEntity();
		//Entity* entity1 = createEntity(entity0);

		////												mesh
		//entity0->meshRenderer.Init(&m_model->m_meshes[5], &material1);
		//entity1->meshRenderer.Init(&m_model->m_meshes[10], &material2);

		//entity0->name = "Tros 0";
		//entity1->name = "Tros 1";

		////entity0->transform.meshScaleFactor = 0.01;
		////entity1->transform.meshScaleFactor = 0.01;

		loadModelMaterials(m_model);
		createModelEntities(m_model);

		/*glActiveTexture(GL_TEXTURE0);
		//Set 
		glUniform1i(glGetUniformLocation(shader.ID(), "aPos"), 0);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, diffuseText.ID());*/



		m_directionalLight.m_intensity = 0;//no directional light
		m_directionalLight.m_dir = glm::normalize(glm::vec3(0, 1, 1));

		LightSource_Point pointLight1(Color::White, 1, glm::vec3(10, .5f, .5));

		pointLight1.m_linear = .35f;
		pointLight1.m_quadratic = .44f;

		m_pointLights.push_back(pointLight1);


		LightSource_Spot spotLight1(Color::White, 1, glm::vec3(0, 2, -2), glm::vec3(0, 0, 1), 5, 10);

		spotLight1.m_linear = .35f;
		spotLight1.m_quadratic = .44f;

		m_spotLights.push_back(spotLight1);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		
		while (!glfwWindowShouldClose(renderer->getWindow()))
		{
			glfwPollEvents();//check events

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			m_directionalLight.setDirection(glm::vec3(cos(dirLight_rotation_X), 0, sin(dirLight_rotation_X)));

			#pragma region Input

			if (input->getKey(Input::KeyboardCode::KEY_CODE_ESCAPE) == Input::KEY_STATE_PRESS)//close application when esc is presseds
				glfwSetWindowShouldClose(renderer->getWindow(), true);

			if (input->getKey(Input::KeyboardCode::KEY_CODE_T) == Input::KEY_STATE_PRESS)
				useTransparency = true;
			if (input->getKey(Input::KeyboardCode::KEY_CODE_Y) == Input::KEY_STATE_PRESS)
				useTransparency = false;

			//Move Camera-----
			bool moveCamera = false;

			if (input->isMouseKeyPressed(Input::MouseButtonCode::MOUSE_CODE_BUTTON_RIGHT)) {
				moveCamera = true;
				input->hideCursor();
			}
			if (input->isMouseKeyReleased(Input::MouseButtonCode::MOUSE_CODE_BUTTON_RIGHT)) {
				moveCamera = false;
				input->showCursor();
			}

			if (moveCamera)
			{
				const float camVelocity = 2;
				float camVelocityMultiplyer = 100;
				const float camRotationSpeed = 2;

				//Camera rotation
				cam.rotate(input->getMouseDeltaX() * camRotationSpeed * deltaTime, input->getMouseDeltaY() * camRotationSpeed * deltaTime);

				//Camera position

				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_LEFT_SHIFT))
					camVelocityMultiplyer = 5;
				if (input->isKeyReleased(Input::KeyboardCode::KEY_CODE_LEFT_SHIFT))
					camVelocityMultiplyer = 1;

				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_W))
					cam.Position += cam.Front * camVelocity * deltaTime * camVelocityMultiplyer;
				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_S))
					cam.Position -= cam.Front * camVelocity * deltaTime * camVelocityMultiplyer;

				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_D))
					cam.Position += cam.Right * camVelocity * deltaTime * camVelocityMultiplyer;
				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_A))
					cam.Position -= cam.Right * camVelocity * deltaTime * camVelocityMultiplyer;

				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_SPACE))
					cam.Position += cam.Up * camVelocity * deltaTime * camVelocityMultiplyer;
				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_LEFT_CONTROL))
					cam.Position -= cam.Up * camVelocity * deltaTime * camVelocityMultiplyer;

			}
#pragma endregion


			renderer->ClearScreen(Color::Grey);


			drawScene(&cam);
			renderGui();


			glfwSwapBuffers(renderer->getWindow());//Update screen

			input->processInput();
		}
	}

	void Engine::sendLightInfo2Shader(Shader& shader, const std::vector<LightSource_Point>& pointLights, std::vector<LightSource_Spot>& spotLights, const LightSource_Directional& dirLight) {

		//Setting directional light
		shader.setFloat("lightsrc_directional_intensity", dirLight.getIntensity());
		shader.setVector("lightsrc_directional_color", dirLight.getColor().getColorVec());
		shader.setVector("lightsrc_directional_direction", dirLight.m_dir);

		shader.setFloat("ambientLight", ambientLight);
		shader.setVector("ambientColor", glm::vec4(1, 1, 1, 1));

		for (int i = 0; i < pointLights.size(); i++)
		{
			std::string pointsi = "pointLights[";
			pointsi += std::to_string(i);
			pointsi += "].";

			//std::cout << (pointsi + "color") << endl;
			/*if(i==0)
				std::cout << pointLights[i].m_intensity<< endl;*/

			shader.setVector(pointsi + "color", pointLights[i].getColor().getColorVec());
			shader.setVector(pointsi + "position", pointLights[i].getPosition());
			shader.setFloat(pointsi + "intensity", pointLights[i].m_intensity);
			shader.setFloat(pointsi + "linear", pointLights[i].m_linear);
			shader.setFloat(pointsi + "quadratic", pointLights[i].m_quadratic);
		}

		for (int i = 0; i < spotLights.size(); i++)
		{
			std::string pointsi = "spotLights[";
			pointsi += std::to_string(i);
			pointsi += "].";

			//std::cout << (pointsi + "color") << endl;
			/*if(i==0)
				std::cout << pointLights[i].m_intensity<< endl;*/

			shader.setVector(pointsi + "color", spotLights[i].getColor().getColorVec());
			shader.setVector(pointsi + "position", spotLights[i].m_pos);
			shader.setVector(pointsi + "direction", normalize(spotLights[i].m_dir));
			shader.setFloat(pointsi + "intensity", spotLights[i].m_intensity);
			shader.setFloat(pointsi + "linear", spotLights[i].m_linear);
			shader.setFloat(pointsi + "angle", std::cos(spotLights[i].m_angle * _PI/180));
			shader.setFloat(pointsi + "outerAngle", std::cos((spotLights[i].m_angle+ spotLights[i].m_outerAngle) * _PI / 180));
		}

		shader.setInt("nPointLights", pointLights.size());
		shader.setInt("nSpotLights", spotLights.size());
	}

	void Engine::sendProjectionInfo2Shader(Shader* shader, Camera* cam)
	{
		glm::mat4 view = glm::mat4(1.0f);

		glm::mat4 projection;

		view = cam->GetViewMatrix();
		projection = glm::perspective(glm::radians(cam->Zoom), renderer->getWindowAspectRatio(), 0.1f, 1000.0f);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVector("viewPos", cam->Position);
	}

	void Engine::drawScene(Camera* cam)
	{

		for (auto entity : entities) {//Opaque pass

			if (!entity->meshRenderer.m_material || !entity->meshRenderer.m_mesh)//doesn't have material or mesh
				continue;

			if (entity->meshRenderer.m_material->isTransparent && useTransparency)
				continue;

			Shader* shader = entity->meshRenderer.m_material->m_shader;
			shader->use();

			glm::mat4 model = entity->transform.globalSpace();

			shader->setMat4("model", model);

			sendLightInfo2Shader(*shader, m_pointLights, m_spotLights, m_directionalLight);
			sendProjectionInfo2Shader(shader, cam);
			entity->meshRenderer.m_material->sendMaterial2Shader();

			entity->meshRenderer.Render();
		}

		if (useTransparency) {
			glEnable(GL_BLEND);

			for (auto entity : entities) {//Transparent pass

				if (!entity->meshRenderer.m_material || !entity->meshRenderer.m_mesh)//doesn't have material or mesh
					continue;

				if (!entity->meshRenderer.m_material->isTransparent)
					continue;

				Shader* shader = entity->meshRenderer.m_material->m_shader;
				shader->use();

				//shader->setBool("use_Transparency", useTransparency);

				glm::mat4 model = entity->transform.globalSpace();

				shader->setMat4("model", model);

				sendLightInfo2Shader(*shader, m_pointLights, m_spotLights, m_directionalLight);
				sendProjectionInfo2Shader(shader, cam);
				entity->meshRenderer.m_material->sendMaterial2Shader();

				entity->meshRenderer.Render();
			}

			glDisable(GL_BLEND);
		}
	}

	void Engine::renderGui()
	{
		imgui_beginFrame();//-----------------------------Render GUI------------------------------------------

		//ImGui::ShowDemoWindow(&show_demo_window);
		ImGui::Begin("Frame Rate");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		//hierarchy
		

		//Light Window
		if (ImGui::Begin("Lights", &show_light_gui)) {
			//Directional light
			if (ImGui::TreeNode("Directional Light")) {

				ImGui::ColorEdit3("Color", (float*)&m_directionalLight.m_color.color_vect);
				ImGui::SliderAngle("X Rotation", &dirLight_rotation_X);
				ImGui::SliderFloat("Intensity", &m_directionalLight.m_intensity, 0, 5);

				ImGui::TreePop();
			}

			//Point Lights
			if (ImGui::TreeNode("Point Lights")) {
				if(m_pointLights.size() < 10)
					if (ImGui::Button("Create New Point Light"))
					{
						LightSource_Point l(Color::White, 1.f, glm::vec3(0.0f));
						l.m_linear = .35f;
						l.m_quadratic = .44f;
						m_pointLights.push_back(l);
					}

				for (int i = 0; i < m_pointLights.size(); i++)
				{
					std::string lightNode = "Light_" + std::to_string(i);
					if (ImGui::TreeNode(lightNode.c_str()))
					{
						ImGui::ColorEdit3("Color", (float*)&m_pointLights[i].m_color.color_vect);
						ImGui::DragFloat3("Position", (float*)&m_pointLights[i].m_pos);

						ImGui::Spacing();

						ImGui::SliderFloat("Intensity", &(m_pointLights[i].m_intensity), 0, 5);
						ImGui::SliderFloat("Linear", &(m_pointLights[i].m_linear), 0, .5);
						ImGui::SliderFloat("Quadratic", &(m_pointLights[i].m_quadratic), 0, .1);
						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}

			//Spot Lights
			if (ImGui::TreeNode("Spot Lights")) {
				if (m_spotLights.size() < 10)
					if (ImGui::Button("Create New Spot Light"))
					{
						LightSource_Spot l(Color::White, 1.f, glm::vec3(0, 10, 0), glm::vec3(0, -1, 0), 5, 5);
						l.m_linear = .35f;
						l.m_quadratic = .44f;
						m_spotLights.push_back(l);
					}

				for (int i = 0; i < m_spotLights.size(); i++)
				{
					std::string lightNode = "Light_" + std::to_string(i);
					if (ImGui::TreeNode(lightNode.c_str()))
					{
						ImGui::ColorEdit3("Color", (float*)&m_spotLights[i].m_color.color_vect);

						ImGui::Spacing();

						ImGui::DragFloat3("Position", (float*)&m_spotLights[i].m_pos);
						ImGui::SliderFloat3("Direction", (float*)&m_spotLights[i].m_dir, -90, 90);

						ImGui::Spacing();

						ImGui::SliderFloat("Intensity", &(m_spotLights[i].m_intensity), 0, 5);

						ImGui::Spacing();

						ImGui::Spacing(); ImGui::SliderFloat("Inner Angle", &(m_spotLights[i].m_angle), 0, 45);
						if (m_spotLights[i].m_outerAngle < m_spotLights[i].m_angle)
							m_spotLights[i].m_outerAngle = m_spotLights[i].m_angle;

						ImGui::Spacing(); ImGui::SliderFloat("Outer Angle", &(m_spotLights[i].m_outerAngle), 0, 45);

						ImGui::Spacing();

						ImGui::SliderFloat("Linear", &(m_spotLights[i].m_linear), 0, .5);
						ImGui::SliderFloat("Quadratic", &(m_spotLights[i].m_quadratic), 0, .1);

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
		}

		ImGui::End();
		imgui_RenderFrame();//**************************************************************
	}

	Entity* Engine::createEntity()
	{
		Entity* entity = m_scene.addChild();
		entities.push_back(entity);
		return entity;
	}
	Entity* Engine::createEntity(Entity* parent)
	{
		Entity* entity = createEntity();
		entity->setParent(parent);
		return entity;
	}

	void Engine::loadModelMaterials(Model* model) {
		for (size_t i = 0; i < model->m_scene->mNumMaterials; i++)
		{
			bool usePhong = false;

			aiMaterial* aMat = model->m_scene->mMaterials[i];
			Material* mat;

			float shinines;
			aiColor3D col;
			aiString path;//diffuse texture relative path

			if (aMat->Get(AI_MATKEY_SHININESS, shinines) == aiReturn::aiReturn_SUCCESS)//Chek if it has specular exponent
			{
				if (shinines != 0) {
					usePhong = true;
					mat = new	Material(shaderManager->getShader("PhongShader"));
				}
				else
				{
					usePhong = true;
					mat = new	Material(shaderManager->getShader("PhongShader"));
				}
			}
			else {
				usePhong = true;
				mat = new	Material(shaderManager->getShader("PhongShader"));
			}


			/*if (usePhong)
			{
				mat->setFloat("shinines", shinines);
				if (aMat->Get(AI_MATKEY_SHININESS_STRENGTH, shinines) == aiReturn::aiReturn_SUCCESS)
					mat->setFloat("specularIntensity", shinines);
				else
				{
					log_printf_info("WARNING::ENGINE::LOAD_MODEL_MATERIAL: failed to get material specular shinines.	 Model: %s\n", model->assetName.c_str());
				}
			}

			if (aMat->Get(AI_MATKEY_COLOR_DIFFUSE, col) == aiReturn::aiReturn_SUCCESS)
				mat->setVec4("myColor", col.r, col.g, col.b, 1.0f);
			else
			{
				log_printf_info("WARNING::ENGINE::LOAD_MODEL_MATERIAL: failed to get material color.	 Model: %s\n", model->assetName.c_str());
			}*/

			mat->setFloat("shinines", 255);
			mat->setFloat("specularIntensity", 1);
			mat->setVec4("myColor", glm::vec4(1.f));
			

			if (aMat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) > 0)
				if (aMat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path) == aiReturn::aiReturn_SUCCESS) {
					std::string textureName = path.C_Str();

					textureName = textureName.substr(0, textureName.find_last_of('.'));//Remove extension

					//log_printf_info("Material[%i] albedoPath: %s", i, textureName.c_str());
					mat->setTexture2D("albedoTexture", textureName);
				}
				else
				{
					log_printf_info("WARNING::ENGINE::LOAD_MODEL_MATERIAL: failed to get material texture.	 Model: %s\n", model->assetName.c_str());
				}

			int blend;
			if (aMat->Get(AI_MATKEY_BLEND_FUNC, blend) == aiReturn::aiReturn_SUCCESS)
				mat->isTransparent = true;

			std::string name = model->assetName + "_Material_";
			name += std::to_string(i);

			mat->name = name.c_str();

			materialManager->AddMaterial(mat);
		}
	}

	void Engine::createModelEntityMeshes(Model* model, aiNode* node, Entity* entity) {

		if (node->mNumMeshes <= 0)
			return;

		Entity* t = entity;
		std::string name = entity->name;
		t->name = name.c_str();

		//Set first mesh
		Mesh* mesh = &model->m_meshes[node->mMeshes[0]];

		aiMesh* amesh = model->m_scene->mMeshes[node->mMeshes[0]];
		std::string matName = model->assetName + "_Material_";
		matName += std::to_string(amesh->mMaterialIndex);


		Material* material = materialManager->getMaterial(matName);

		if (material == nullptr)
			log_error("WARNING::ENGINE::FAILED_TO_CREATE_ENTITY_MESH: material not found");

		t->meshRenderer.Init(mesh, material);

		for (size_t i = 1; i < node->mNumMeshes; i++)//set other meshes
		{
			t = createEntity(t);
			std::string name = entity->name;
			name += "_";
			name += std::to_string(i);
			t->name = name.c_str();

			Mesh* mesh = &model->m_meshes[node->mMeshes[i]];

			aiMesh* amesh = model->m_scene->mMeshes[node->mMeshes[i]];
			std::string matName = model->assetName + "_Material_";
			matName += std::to_string(amesh->mMaterialIndex);

			Material* material = materialManager->getMaterial(matName);

			if (material == nullptr)
				log_error("WARNING::ENGINE::FAILED_TO_CREATE_ENTITY_MESH: material not found");

			t->meshRenderer.Init(mesh, material);
		}
	}

	void Engine::createModelEntity(Model* model, aiNode* parentNode, Entity* parent) {
		
		for (size_t i = 0; i < parentNode->mNumChildren; i++)
		{
			aiNode* node = parentNode->mChildren[i];
			Entity* entity = createEntity(parent);
			entity->name = node->mName.C_Str();
			entity->transform.meshScaleFactor = .001f;

			createModelEntityMeshes(model, node, entity);

			createModelEntity(model,node, entity);
		}
	}

	Entity* Engine::createModelEntities(Model* model)
	{
		//TODO: get model transform. current implementation of model loses the transform hierarchy

		aiNode* nRoot = model->m_scene->mRootNode;

		Entity* eRoot = createEntity();
		
		eRoot->name = nRoot->mName.C_Str();

		createModelEntityMeshes(model, nRoot, eRoot);

		createModelEntity(model, nRoot, eRoot);

		return NULL;
	}
}


