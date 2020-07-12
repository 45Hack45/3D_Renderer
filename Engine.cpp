#include "Engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IO.hpp"
#include <stdlib.h>
#include "Texture.h"
#include "Camera.h"
#include "Input.h"

namespace Engine
{
	int Engine::Init(const char* projectPath, const char* projectName) {
		log_message(log_level_e::LOG_INFO, "Initializing Engine");
		IO::projectPath = projectPath;

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

		log_message(log_level_e::LOG_INFO, "Engine Initialized\n");
	}

	void Engine::Start() {
		m_model = new Model("./rcs/backpack/backpack.obj", "Backpack");

		float iniTime = glfwGetTime();
		m_model->loadFile();
		float finTime = glfwGetTime();
		std::cout << "Load Backpack model time: " << finTime - iniTime << std::endl << std::endl;
	}

	void Engine::mainLoop() {
		log_message(log_level_e::LOG_DEBUG, "Starting Main Loop\n");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		log_message(log_level_e::LOG_DEBUG, "Loading Shader\n");

		const char* vertPath = "./Shaders/UnlitShader.vert";
		const char* fragPath = "./Shaders/UnlitShader.frag";

		Shader shader(vertPath, fragPath);

		shader.use();

		//Camera
		Camera cam = Camera(glm::vec3(0, 0, 3));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 view = glm::mat4(1.0f);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(cam.Zoom), renderer->getWindowAspectRatio(), 0.1f, 100.0f);

		glEnable(GL_DEPTH_TEST);


		float iniTime = glfwGetTime();
		Texture diffuseText("./rcs/backpack/diffuse.jpg");
		//Texture aoText("./rcs/backpack/ao.jpg");
		//Texture normalText("./rcs/backpack/normal.png");
		//Texture roughnessText("./rcs/backpack/roughness.jpg");
		//Texture specularText("./rcs/backpack/specular.jpg");
		float finTime = glfwGetTime();

		std::cout << "Load Textures time: " << finTime - iniTime << std::endl << std::endl;


		glActiveTexture(GL_TEXTURE0);
		//Set 
		glUniform1i(glGetUniformLocation(shader.ID(), "aPos"), 0);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, diffuseText.ID());

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
				const float camRotationSpeed = 2;

				//Camera rotation
				cam.rotate(input->getMouseDeltaX() * camRotationSpeed * deltaTime, input->getMouseDeltaY() * camRotationSpeed * deltaTime);

				//Camera position
				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_W))
					cam.Position += cam.Front * camVelocity * deltaTime;
				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_S))
					cam.Position -= cam.Front * camVelocity * deltaTime;

				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_D))
					cam.Position += cam.Right * camVelocity * deltaTime;
				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_A))
					cam.Position -= cam.Right * camVelocity * deltaTime;

				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_SPACE))
					cam.Position += cam.Up * camVelocity * deltaTime;
				if (input->isKeyPressed(Input::KeyboardCode::KEY_CODE_LEFT_CONTROL))
					cam.Position -= cam.Up * camVelocity * deltaTime;
			}


			renderer->ClearScreen(Color::Grey);

			shader.use();

			view = cam.GetViewMatrix();
			projection = glm::perspective(glm::radians(cam.Zoom), renderer->getWindowAspectRatio(), 0.1f, 100.0f);

			shader.setMat4("model", model);
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);
			shader.setVector("viewPos", cam.Position);

			m_model->Draw();

			glfwSwapBuffers(renderer->getWindow());//Update screen

			input->processInput();
		}
	}
}


