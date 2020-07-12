#pragma once

#include "glad.h"
#include "glfw3.h"

#include "stb_image.h"

#include "logger.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "LightSource.h"

#define window_size_X 400
#define window_size_Y 300
#define aRatio 1.f*window_size_X/window_size_Y
#define window_size  window_size_X,window_size_Y

#define full_screen_mode false

#define _pi 3.14159265359

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//Camera
Camera cam = Camera(glm::vec3(0, 0, 3));

bool moveCam = false;

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "ERROR::GLFW	%d: %s\n", error, description);
}

//Resize viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (moveCam) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	static float lastX = window_size_X / 2.0f;
	static float lastY = window_size_Y / 2.0f;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	if (moveCam)
		cam.ProcessMouseMovement(xoffset, yoffset);

	//std::cout << xpos << ",	" << ypos << std::endl;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//Hide cursor
		moveCam = true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//Show cursor
		moveCam = false;
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (moveCam)
		cam.ProcessMouseScroll(yoffset);
}

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

void sendLightInfo2Shader(Shader& shader, const std::vector<LightSource_Point>& pointLights, std::vector<LightSource_Spot>& spotLights, const LightSource_Directional& dirLight) {

	shader.use();

	//Setting directional light
	shader.setFloat("lightsrc_directional_intensity", dirLight.getIntensity());
	shader.setVector("lightsrc_directional_color", dirLight.getColor().getColorVec());

	shader.setFloat("ambientLight", .025f);
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
		shader.setFloat(pointsi + "angle", cos(spotLights[i].m_angle * _pi));
		shader.setFloat(pointsi + "outerAngle", cos(spotLights[i].m_outerAngle * _pi));
	}

	shader.setInt("nPointLights", pointLights.size());
	shader.setInt("nSpotLights", spotLights.size());
}

int test_render() {

	//Initializing window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

	GLFWwindow* window = glfwCreateWindow(window_size_X, window_size_Y, "Renderer", (full_screen_mode) ? primaryMonitor : NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Seting window
	glfwMakeContextCurrent(window);

	//Init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	glViewport(0, 0, window_size);//Screen size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//Telling glad to call framebuffer_size_callback on resize window
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//hide and auto-center cursor


	initImgui(window);



	set_log_level(LOG_DEBUG);
	log_message(LOG_INFO, "//-----------------Starting Test-----------------//");

	// load and compile our shader program
	// ------------------------------------
	const char* vertPath = "./Shaders/PhongShader.vert";
	const char* fragPath = "./Shaders/PhongShader.frag";

	Shader shader = Shader(vertPath, fragPath);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Set shader sampler indices
	shader.use();
	//shader.setInt("texture1", 0);
	//shader.setInt("texture2", 1);


	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(cam.Zoom), aRatio, 0.1f, 100.0f);

	glEnable(GL_DEPTH_TEST);

	Test::Model mesh("./rcs/backpack/backpack.obj");

	//Lights

	//Directional Light
	LightSource_Directional dirLight(Color(252, 212, 64, 1) / 255.f, 1.f, glm::vec3(1, 0, 0));

	std::vector<LightSource_Point> pointLights;
	std::vector<LightSource_Spot> spotLights;

	//PointLight
	LightSource_Point pointL1(Color(0.f, 0.f, 1.f), 3.f, glm::vec3(0, 0, 10), 0.14, 0.07);
	LightSource_Point pointLRed(Color(1.f, 0, 0), 3.f, glm::vec3(0, 0, -10), 0.14, 0.07);

	pointLights.push_back(pointL1);
	pointLights.push_back(pointLRed);

	//SpotLight
	LightSource_Spot spot1(Color(1.f, 1.f, 1.f), 5.f, glm::vec3(0, 10, 2), glm::vec3(0, -10, -1.957), .026, 50, 0.14, 0.07);

	spotLights.push_back(spot1);

	bool show_dir_light_gui = true;
	float rotationSpeed = .25f;
	float dirLight_rotation_X = 0, dirLight_rotation_Y = 0;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//Clearing screen
		glClearColor(1.f, 0.3f, 0.3f, 1.0f);
		//glClearColor(.0f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;//Changing the value over time

		shader.use();//Bind shader program

		//Rotate over time
		const float radius = 5.f;
		const float frequency = 1.f;
		float camX = sin(glfwGetTime() * frequency) * radius;
		float camZ = cos(glfwGetTime() * frequency) * radius;

		sendLightInfo2Shader(shader, pointLights, spotLights, dirLight);//Update Lights


		view = cam.GetViewMatrix();
		projection = glm::perspective(glm::radians(cam.Zoom), aRatio, 0.1f, 100.0f);

		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVector("viewPos", cam.Position);

		//Update directional light direction
		//dirLight_direction = glm::vec3(sin(glfwGetTime() * rotationSpeed), 0, cos(glfwGetTime() * rotationSpeed));
		dirLight.setDirection(glm::vec3(cos(dirLight_rotation_X), 0, sin(dirLight_rotation_X)));
		shader.setVector("lightsrc_directional_direction", dirLight.getDirection(glm::vec3(.0f)));

		mesh.Draw(shader);

		imgui_beginFrame();//-----------------------------Render GUI------------------------------------------

		//ImGui::ShowDemoWindow(&show_demo_window);
		ImGui::Begin("Frame Rate");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		//Light Windows
		if (ImGui::Begin("Lights", &show_dir_light_gui)) {
			//Directional light
			if (ImGui::TreeNode("Directional Light")) {

				ImGui::ColorEdit3("Color", (float*)&dirLight.m_color.color_vect);
				ImGui::SliderAngle("X Rotation", &dirLight_rotation_X);
				ImGui::SliderFloat("Intensity", &dirLight.m_intensity, 0, 5);

				ImGui::TreePop();
			}

			//Point Lights
			if (ImGui::TreeNode("Point Lights")) {

				for (int i = 0; i < pointLights.size(); i++)
				{
					std::string lightNode = "Light_" + std::to_string(i);
					if (ImGui::TreeNode(lightNode.c_str()))
					{
						ImGui::ColorEdit3("Color", (float*)&pointLights[i].m_color.color_vect);
						ImGui::DragFloat3("Position", (float*)&pointLights[i].m_pos);

						ImGui::Spacing();

						ImGui::SliderFloat("Intensity", &(pointLights[i].m_intensity), 0, 5);
						ImGui::SliderFloat("Linear", &(pointLights[i].m_linear), 0, .5);
						ImGui::SliderFloat("Quadratic", &(pointLights[i].m_quadratic), 0, .1);
						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}

			//Spot Lights
			if (ImGui::TreeNode("Spot Lights")) {

				for (int i = 0; i < spotLights.size(); i++)
				{
					std::string lightNode = "Light_" + std::to_string(i);
					if (ImGui::TreeNode(lightNode.c_str()))
					{
						ImGui::ColorEdit3("Color", (float*)&spotLights[i].m_color.color_vect);

						ImGui::Spacing();

						ImGui::DragFloat3("Position", (float*)&spotLights[i].m_pos);
						ImGui::SliderFloat3("Direction", (float*)&spotLights[i].m_dir, -90, 90);

						ImGui::Spacing();

						ImGui::SliderFloat("Intensity", &(spotLights[i].m_intensity), 0, 5);

						ImGui::Spacing();

						ImGui::Spacing(); ImGui::SliderFloat("Inner Angle", &(spotLights[i].m_angle), 0, 1);
						ImGui::Spacing(); ImGui::SliderFloat("Outer Angle", &(spotLights[i].m_outerAngle), 0, 1);

						ImGui::Spacing();

						ImGui::SliderFloat("Linear", &(spotLights[i].m_linear), 0, .5);
						ImGui::SliderFloat("Quadratic", &(spotLights[i].m_quadratic), 0, .1);

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
		}
		ImGui::End();
		imgui_RenderFrame();//**************************************************************

		glfwSwapBuffers(window);//Update screen
		glfwPollEvents();//check events
	}

	glfwTerminate();//Clear resources
}

