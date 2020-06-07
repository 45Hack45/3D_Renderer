#include "include/glad.h"
#include "include/glfw3.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

#include"logger.h"

#include "Shader.h"
#include "Texture.h"

#define window_size_X 800
#define window_size_Y 600
#define window_size  window_size_X,window_size_Y

//Resize viewport
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void test_render_triangle(GLFWwindow* window) {

	set_log_level(LOG_DEBUG);
	log_message(LOG_INFO, "//-----------------Starting Test-----------------//");

	// load and compile our shader program
	// ------------------------------------
	const char *vertPath = "./Shaders/test.vert";
	const char* fragPath = "./Shaders/test.frag";

	Shader shader = Shader(vertPath, fragPath);
	

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	// load and generate the textures
	Texture texture1 = Texture("./rcs/container.jpg");
	Texture texture2 = Texture("./rcs/awesomeface.png");

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 3, 2,
		0, 2, 1
	};

	float texCoords[] = {
		0.0f, 0.0f,  // lower-left corner  
		1.0f, 0.0f,  // lower-right corner
		0.5f, 1.0f   // top-center corner
	};
	

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);//Create vertex buffer
	glGenBuffers(1, &EBO);//create triangle buffer
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);//Bind buffer
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);//Send data to gpu

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//Bind buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//Send data to gpu

	//-----------------------set the vertex attributes pointer. Indicate to OpenGL how to read the data------------------------
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//uv's
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);//unbinding VBO

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);//unbinding VAO

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Set shader sampler indices
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);
	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		//Clearing screen
		glClearColor(1.f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;//Changing the value over time
		
		shader.use();//Bind shader program
		shader.setFloat("ourColor", 0.0f, greenValue, 0.0f, 1.0f);//setting uniform color

		glActiveTexture(GL_TEXTURE0);// activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, texture1.ID());//bind texture

		glActiveTexture(GL_TEXTURE1);// activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, texture2.ID());//bind texture
		
		glBindVertexArray(VAO);
		
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);//Update screen
		glfwPollEvents();//check events
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

int main() {

	//Initializing window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_size_X, window_size_Y, "LearnOpenGL", NULL, NULL);
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


	test_render_triangle(window);

	//Main loop
	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		//Clearing screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(window);//Update screen
		glfwPollEvents();//check events
	}


	glfwTerminate();//Clear resources
	return 0;
}
