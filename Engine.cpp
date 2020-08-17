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
#include "FrameBuffer.h"


#define unlitShaderVert "./Shaders/UnlitShader.vert"
#define unlitShaderFrag "./Shaders/UnlitShader.frag"

#define phongShaderVert "./Shaders/PhongShader.vert"
#define phongShaderFrag "./Shaders/PhongShader.frag"

#define useUnlitShader false

#define ambientLight .05f

float shadowMap_Resolution = 1024 * 1;

namespace Engine
{
	const int _PI = 3.14159265359;
	FrameBuffer*	shadow_fbo;

	Camera auxCam;
	glm::vec3 auxPosOffset;
	float shadowBias = 0, shadowBiasMax = 8;

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

		std::cout << "Resolucio Shadowmap: 1024x";
		std::cin >> shadowMap_Resolution;

		shadow_fbo = new FrameBuffer(shadowMap_Resolution*1024, shadowMap_Resolution*1024, FrameBuffer::FrameType_Depth | FrameBuffer::FrameType_Color, 0);

		log_message(log_level_e::LOG_INFO, "Engine Initialized\n");
	}

	void Engine::Start() {
		//m_model = new Model("./rcs/sponza/Sponza.gltf", "Backpack");

		m_model = modelManager->getModel("Sponza",false, true);//Geting and loading model
		m_model->loadFile(.05f);

		m_model2 = modelManager->getModel("backpack", false);
		m_model2->loadFile(1.f);

		//IO::printEngineRCSFiles();
		//IO::printProjectFiles();
	}

	void Engine::mainLoop() {
		log_message(log_level_e::LOG_DEBUG, "\n\n\n--------------------------Starting Main Loop------------------------\n");

		//Camera
		Camera cam = Camera(glm::vec3(-20, 10, 0));
		cam.setYaw(0);

		auxCam.ortoFrustrum_Horizontal = 50.f;
		auxCam.ortoFrustrum_Vertical = 50.f;
		auxCam.isOrtographic = true;
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(glm::rotate(model, glm::radians(-45.0f * 0), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.f, 1.f, 1.f)*1.f);

		scene->loadModel2Scene(m_model);
		Entity* m2Entity = scene->loadModel2Scene(m_model2);


		m2Entity->transform.m_position = glm::vec3(0, 10, 0);

		scene->m_directionalLight.setDirection(0.f, 100.f);
		scene->m_directionalLight.m_intensity = 1.f;

		LightSource_Point pointLight1(Color::White, 1, glm::vec3(54, 2.5f, 5.5));

		pointLight1.m_linear = .038f;
		pointLight1.m_quadratic = .024f;
		scene->m_pointLights.push_back(pointLight1);


		LightSource_Spot spotLight1(Color::White, 1, glm::vec3(0, 10, -2), glm::vec3(0, 0, 1), 10, 13);

		spotLight1.m_linear = .156f;
		spotLight1.m_quadratic = .0144f;
		scene->m_spotLights.push_back(spotLight1);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		Shader* shader = shaderManager->getShader("PhongShader");
		shader->bind();
		

		while (!glfwWindowShouldClose(renderer->getWindow()))
		{
			glfwPollEvents();//check events

			currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			//scene->m_directionalLight.setDirection(glm::vec3(cos(editor->dirLight_rotation_X), 0, sin(editor->dirLight_rotation_X)));

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
				const float camVelocity = 10;
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

			imgui_beginFrame_();//--------------------------------------------------------------

			renderer->ClearScreen(Color::Grey);

			drawScene(&cam);
			
			editor->renderGui(deltaTime, &scene->m_scene, scene);
			
			imgui_RenderFrame();//**************************************************************

			glfwSwapBuffers(renderer->getWindow());//Update screen

			input->processInput();
		}
	}

	void Engine::sendLightInfo2Shader(Shader* shader, const std::vector<LightSource_Point>& pointLights, std::vector<LightSource_Spot>& spotLights, const LightSource_Directional& dirLight) {

		//Setting directional light
		shader->setFloat("lightsrc_directional_intensity", dirLight.getIntensity());
		shader->setVector("lightsrc_directional_color", dirLight.getColor().getColorVec());
		shader->setVector("lightsrc_directional_direction", dirLight.getDirection());

		shader->setFloat("ambientLight", ambientLight);
		shader->setVector("ambientColor", glm::vec4(1, 1, 1, 1));

		for (int i = 0; i < pointLights.size(); i++)
		{
			std::string pointsi = "pointLights[";
			pointsi += std::to_string(i);
			pointsi += "].";

			//std::cout << (pointsi + "color") << endl;
			/*if(i==0)
				std::cout << pointLights[i].m_intensity<< endl;*/

			shader->setVector(pointsi + "color", pointLights[i].getColor().getColorVec());
			shader->setVector(pointsi + "position", pointLights[i].getPosition());
			shader->setFloat(pointsi + "intensity", pointLights[i].m_intensity);
			shader->setFloat(pointsi + "linear", pointLights[i].m_linear);
			shader->setFloat(pointsi + "quadratic", pointLights[i].m_quadratic);
		}

		for (int i = 0; i < spotLights.size(); i++)
		{
			std::string pointsi = "spotLights[";
			pointsi += std::to_string(i);
			pointsi += "].";

			//std::cout << (pointsi + "color") << endl;
			/*if(i==0)
				std::cout << pointLights[i].m_intensity<< endl;*/

			shader->setVector(pointsi + "color", spotLights[i].getColor().getColorVec());
			shader->setVector(pointsi + "position", spotLights[i].m_pos);
			shader->setVector(pointsi + "direction", normalize(spotLights[i].m_dir));
			shader->setFloat(pointsi + "intensity", spotLights[i].m_intensity);
			shader->setFloat(pointsi + "linear", spotLights[i].m_linear);
			shader->setFloat(pointsi + "quadratic", spotLights[i].m_quadratic);
			shader->setFloat(pointsi + "angle", std::cos(spotLights[i].m_angle * _PI/180));
			shader->setFloat(pointsi + "outerAngle", std::cos((spotLights[i].m_angle+ spotLights[i].m_outerAngle) * _PI / 180));
		}

		shader->setInt("nPointLights", pointLights.size());
		shader->setInt("nSpotLights", spotLights.size());
	}

	void Engine::sendShadowInfo2Shader(Shader* shader) {
		shader->setInt("shadowMap", 15);//set shadowMap sampler to active texture 15
		shadow_fbo->bindDepthTexture(15);//bind depth texture to active texture 15

		glm::mat4 shadowMatrix = auxCam.GetProjectionMatrix() * auxCam.GetViewMatrix();
		shader->setMat4("shadowMatrix", shadowMatrix);
		shader->setFloat("shadowBias", shadowBias * .00001f);
		shader->setFloat("shadowBiasMax", shadowBiasMax * .00001f);
	}

	void Engine::sendProjectionInfo2Shader(Shader* shader, Camera* cam)
	{
		glm::mat4 view = glm::mat4(1.0f);

		glm::mat4 projection = glm::mat4(1.f);

		view = cam->GetViewMatrix();

		float near = .01f;
		float far = 1000.0f;

		projection *= cam->GetProjectionMatrix();

		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->setVector("viewPos", cam->Position);
	}

	void Engine::drawScene(Camera* cam)
	{
		
		static float auxCamDistance = 500.f;

		if (ImGui::Begin("Shadowmap")) {
			ImGui::DragFloat3("AuxCam Offset", (float*)&auxPosOffset);

			ImGui::DragFloat("Horizontal", &auxCam.ortoFrustrum_Horizontal);
			ImGui::DragFloat("Vertical", &auxCam.ortoFrustrum_Vertical);

			ImGui::DragFloat("Near", &auxCam.near);
			ImGui::DragFloat("Far", &auxCam.far);

			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::DragFloat("ShadowBiasFactor", &shadowBias);
			ImGui::DragFloat("ShadowBiasMax", &shadowBiasMax);
			ImGui::DragFloat("AuxCam Distance", &auxCamDistance);
		}
		ImGui::End();

		LightSource_Spot* spotLight = &scene->m_spotLights[0];
		LightSource_Directional* dirlight = &scene->m_directionalLight;

		dirlight->updateDirection();
		glm::vec3 dir = -glm::normalize(dirlight->getDirection());

		auxCam.Position = cam->Position;
		auxCam.Position.y = 0;
		auxCam.Position += auxPosOffset  - dir * auxCamDistance;

		auxCam.setFront(dir);


		float tH = auxCam.ortoFrustrum_Horizontal;
		float tV = auxCam.ortoFrustrum_Vertical;
		float tNear = auxCam.near;
		float tFar = auxCam.far;
		//Shadowmap pass
		shadow_fbo->bind(true);

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderPass(&auxCam, shaderManager->getShader("ShadowMapShader"));

		shadow_fbo->unbind();

		glViewport(0, 0, renderer->getWindowSizeX(), renderer->getWindowSizeY());


		static bool showShadowCam = false;

		if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_Q))
			showShadowCam = true;
		if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_E))
			showShadowCam = false;

		if(showShadowCam)
			drawFullScreenQuad(shadow_fbo->colorTextureID());

		Shader* phongShader = shaderManager->getShader("PhongShader");
		phongShader->bind();


		sendShadowInfo2Shader(phongShader);

		sendLightInfo2Shader(phongShader, scene->m_pointLights, scene->m_spotLights, scene->m_directionalLight);

		//opaque pass
		renderPass(cam, [](Entity* entity, Shader* shader) {
			return !entity->meshRenderer.m_material->isTransparent; 
		});


		//transparent pass

		//glEnable(GL_BLEND);
		renderPass(cam, [](Entity* entity, Shader* shader) {
			return entity->meshRenderer.m_material->isTransparent;
		});
		//glDisable(GL_BLEND);
	}

	void Engine::drawFullScreenQuad(unsigned int textureID){

		float vertices[] = {
			//Position		
			 1,  1, 0.0f,	1, 1,		// top right
			 1, -1, 0.0f,	1, 0,	// bottom right
			-1, -1, 0.0f,	0, 0,	// bottom left
			-1,  1, 0.0f,	0, 1	// top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			2,0,3,
			2,1,0
		};
		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// uv attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);


		Shader* shader = shaderManager->getShader("FullScreenQuadShader");
		shader->bind();

		//bind texture
		glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, textureID);


		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void Engine::renderPass(Camera* cam, RenderEntity_Callback func){

		for (auto entity : scene->entities) {

			if (!entity->meshRenderer.m_material || !entity->meshRenderer.m_mesh)//doesn't have material or mesh
				continue;

			Shader* shader = entity->meshRenderer.m_material->m_shader;
			shader->bind();

			if (!func(entity, shader))//evaluate the entity
				continue;

			glm::mat4 model = entity->transform.globalSpace();

			shader->setMat4("model", model);

			sendProjectionInfo2Shader(shader, cam);
			entity->meshRenderer.m_material->sendMaterial2Shader();

			entity->meshRenderer.Render();
		}
	}
	void Engine::renderPass(Camera* cam, Shader* shader, RenderEntity_Callback func){

		for (auto entity : scene->entities) {

			if (!entity->meshRenderer.m_material || !entity->meshRenderer.m_mesh)//doesn't have material or mesh
				continue;

			shader->bind();

			if (!func(entity, shader))//evaluate the entity
				continue;

			glm::mat4 model = entity->transform.globalSpace();

			shader->setMat4("model", model);

			sendProjectionInfo2Shader(shader, cam);
			entity->meshRenderer.m_material->sendMaterial2Shader();

			entity->meshRenderer.Render();
		}
	}
}


