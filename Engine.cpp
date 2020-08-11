#include "Engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/imgui/imgui.h"
#include "include/imgui/examples/imgui_impl_glfw.h"
#include "include/imgui/examples/imgui_impl_opengl3.h"

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
	const int _PI = 3.14159265359;

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

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

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

		//Init editor
		editor = Editor::Instance();
		editor->Init();

		scene = new Scene();//Init scene

		log_message(log_level_e::LOG_INFO, "Engine Initialized\n");
	}

	void Engine::Start() {
		//m_model = new Model("./rcs/sponza/Sponza.gltf", "Backpack");

		m_model = modelManager->getModel("Sponza",false);//Geting and loading model
		m_model->loadFile();

		//IO::printEngineRCSFiles();
		//IO::printProjectFiles();
	}

	void Engine::mainLoop() {
		log_message(log_level_e::LOG_DEBUG, "\n\n\n--------------------------Starting Main Loop------------------------\n");

		//Camera
		Camera cam = Camera(glm::vec3(0, 1, 5));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(glm::rotate(model, glm::radians(-45.0f * 0), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.f, 1.f, 1.f)*1.f);

		scene->loadModel2Scene(m_model);


		scene->m_directionalLight.m_intensity = .75;
		scene->m_directionalLight.m_dir = glm::normalize(glm::vec3(0, 1, 1));

		LightSource_Point pointLight1(Color::White, 1, glm::vec3(10, .5f, .5));

		pointLight1.m_linear = .35f;
		pointLight1.m_quadratic = .44f;
		scene->m_pointLights.push_back(pointLight1);


		LightSource_Spot spotLight1(Color::White, 1, glm::vec3(0, 2, -2), glm::vec3(0, 0, 1), 5, 10);

		spotLight1.m_linear = .35f;
		spotLight1.m_quadratic = .44f;
		scene->m_spotLights.push_back(spotLight1);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		
		while (!glfwWindowShouldClose(renderer->getWindow()))
		{
			glfwPollEvents();//check events

			currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			scene->m_directionalLight.setDirection(glm::vec3(cos(editor->dirLight_rotation_X), 0, sin(editor->dirLight_rotation_X)));

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
			editor->renderGui(deltaTime, &scene->m_scene, scene);


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

		for (auto entity : scene->entities) {//Opaque pass

			if (!entity->meshRenderer.m_material || !entity->meshRenderer.m_mesh)//doesn't have material or mesh
				continue;

			if (entity->meshRenderer.m_material->isTransparent && useTransparency)
				continue;

			Shader* shader = entity->meshRenderer.m_material->m_shader;
			shader->use();

			glm::mat4 model = entity->transform.globalSpace();

			shader->setMat4("model", model);

			sendLightInfo2Shader(*shader, scene->m_pointLights, scene->m_spotLights, scene->m_directionalLight);
			sendProjectionInfo2Shader(shader, cam);
			entity->meshRenderer.m_material->sendMaterial2Shader();

			entity->meshRenderer.Render();
		}

		if (useTransparency) {
			glEnable(GL_BLEND);

			for (auto entity : scene->entities) {//Transparent pass

				if (!entity->meshRenderer.m_material || !entity->meshRenderer.m_mesh)//doesn't have material or mesh
					continue;

				if (!entity->meshRenderer.m_material->isTransparent)
					continue;

				Shader* shader = entity->meshRenderer.m_material->m_shader;
				shader->use();

				//shader->setBool("use_Transparency", useTransparency);

				glm::mat4 model = entity->transform.globalSpace();

				shader->setMat4("model", model);

				sendLightInfo2Shader(*shader, scene->m_pointLights, scene->m_spotLights, scene->m_directionalLight);
				sendProjectionInfo2Shader(shader, cam);
				entity->meshRenderer.m_material->sendMaterial2Shader();

				entity->meshRenderer.Render();
			}

			glDisable(GL_BLEND);
		}
	}
}


