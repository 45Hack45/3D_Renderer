#include "Engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IO.h"
#include <stdlib.h>
#include "Texture.h"
#include "Camera.h"
#include "Input.h"

#define unlitShaderVert "./Shaders/UnlitShader.vert"
#define unlitShaderFrag "./Shaders/UnlitShader.frag"

#define phongShaderVert "./Shaders/PhongShader.vert"
#define phongShaderFrag "./Shaders/PhongShader.frag"

#define useUnlitShader false

#define ambientLight .025

namespace Engine
{
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

		input = Input::Input::Instance();
		input->Init(&renderer->m_window);

		IO::scanFiles();

		shaderManager = ShaderManager::Instance();
		shaderManager->Init(false);

		textureManager = TextureManager::Instance();
		textureManager->Init(false);

		modelManager = ModelManager::Instance();
		modelManager->Init(false);

		log_message(log_level_e::LOG_INFO, "Engine Initialized\n");
	}

	void Engine::Start() {
		//m_model = new Model("./rcs/sponza/Sponza.gltf", "Backpack");

		m_model = modelManager->getModel("Sponza");

		float iniTime = glfwGetTime();
		m_model->loadFile();
		float finTime = glfwGetTime();
		std::cout << "Load Backpack model time: " << finTime - iniTime << std::endl << std::endl;

		//IO::printProjectFiles();
	}

	void Engine::mainLoop() {
		log_message(log_level_e::LOG_DEBUG, "\n\n\n--------------------------Starting Main Loop------------------------\n");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//log_message(log_level_e::LOG_DEBUG, "Loading Shader\n");

		const char* vertPath = (useUnlitShader) ? unlitShaderVert : phongShaderVert;
		const char* fragPath = (useUnlitShader) ? unlitShaderFrag : phongShaderFrag;

		//Shader shader(vertPath, fragPath);
		//Shader shader("./Shaders/PhongShader.glsl");

		//Shader shader = shaderManager->getShader("PhongShader");

		Shader shader = *shaderManager->getShader("PhongShader");
		
		log_printf(log_level_e::LOG_INFO, "Shader error %d", shader.errorOnLoad);

		shader.use();

		//Camera
		Camera cam = Camera(glm::vec3(0, 0, 3));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(glm::rotate(model, glm::radians(-45.0f * 0), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.f, 1.f, 1.f)*.01f);

		glm::mat4 view = glm::mat4(1.0f);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(cam.Zoom), renderer->getWindowAspectRatio(), 0.1f, 10000.0f);



		float iniTime = glfwGetTime();
		//Texture diffuseText("./rcs/backpack/diffuse.jpg");
		//Texture aoText("./rcs/backpack/ao.jpg");
		//Texture normalText("./rcs/backpack/normal.png");
		//Texture roughnessText("./rcs/backpack/roughness.jpg");
		//Texture specularText("./rcs/backpack/specular.jpg");
		
		//Texture grassText("./rcs/Amaryllis City/OBJ/Amaryllis City/Grass.jpg");
		
		Texture wallTexture = *textureManager->getTexture("wall");

		float finTime = glfwGetTime();

		std::cout << "Load Textures time: " << finTime - iniTime << std::endl << std::endl;


		/*glActiveTexture(GL_TEXTURE0);
		//Set 
		glUniform1i(glGetUniformLocation(shader.ID(), "aPos"), 0);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, diffuseText.ID());*/

		//m_directionalLight.m_intensity = 0;//no directional light
		m_directionalLight.m_dir = glm::normalize(glm::vec3(1, 1, 1));

		LightSource_Point pointLight1(Color::White, 1, glm::vec3(0, 0, 2));

		pointLight1.m_linear = .045f;
		pointLight1.m_quadratic = .0075;

		m_pointLights.push_back(pointLight1);


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		
		while (!glfwWindowShouldClose(renderer->getWindow()))
		{
			glfwPollEvents();//check events

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;


			if (input->getKey(Input::KeyboardCode::KEY_CODE_ESCAPE) == Input::KEY_STATE_PRESS)//close application when esc is presseds
				glfwSetWindowShouldClose(renderer->getWindow(), true);

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
				float camVelocityMultiplyer = 10;
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


			renderer->ClearScreen(Color::Grey);

			for (int i = 0; i < 1; i++)
			{
				shader.use();
				view = cam.GetViewMatrix();
				projection = glm::perspective(glm::radians(cam.Zoom), renderer->getWindowAspectRatio(), 0.1f, 100.0f);

				glm::mat4 model2 = glm::mat4(1.0f);
				model2 = glm::translate(model, glm::vec3(i * 3.5f, 0, -i*2.f));

				shader.setMat4("model", model2);
				shader.setMat4("view", view);
				shader.setMat4("projection", projection);
				shader.setVector("viewPos", cam.Position);

				sendLightInfo2Shader(shader, m_pointLights, m_spotLights, m_directionalLight);

				m_model->Draw();
			}

			glfwSwapBuffers(renderer->getWindow());//Update screen

			input->processInput();
		}
	}

	void Engine::sendLightInfo2Shader(Shader& shader, const std::vector<LightSource_Point>& pointLights, std::vector<LightSource_Spot>& spotLights, const LightSource_Directional& dirLight) {

		shader.use();

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
			shader.setFloat(pointsi + "angle", cos(spotLights[i].m_angle * _PI));
			shader.setFloat(pointsi + "outerAngle", cos(spotLights[i].m_outerAngle * _PI) + cos(spotLights[i].m_angle * _PI));
		}

		shader.setInt("nPointLights", pointLights.size());
		shader.setInt("nSpotLights", spotLights.size());
	}

}


