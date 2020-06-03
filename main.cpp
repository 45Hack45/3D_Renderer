#include "include/glad.h"
#include "include/glfw3.h"
#include <iostream>

#define window_size_X 800
#define window_size_Y 600
#define window_size  window_size_X,window_size_Y

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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

	// build and compile our shader program
	// ------------------------------------

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//Create shader

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//Set src code
	glCompileShader(vertexShader);//Compile vertex Shader

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//Check error
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	std::cout << "Compiled Vertex Shader\n";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//Create frag shader

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//set src code
	glCompileShader(fragmentShader);//Compile

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//Check error
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	std::cout << "Compiled Fragment Shader\n";

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();//Creating shader program

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);//Linking vertex and frag shaders

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::FAILED_LINKING\n" << infoLog << std::endl;

		return;
	}

	//Clear recources
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::cout << "Shader Program Linked\n";

	// ------------------------------------


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	GLfloat vertices[] = {
	0.5f,  0.5f, 0.0f,  // top right
	0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left
	};

	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
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

	//set the vertex attributes pointer. Indicate to OpenGL how to read the data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);//unbinding VBO

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);//unbinding VAO

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		//Clearing screen
		glClearColor(1.f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);//Bind shader program
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);//Update screen
		glfwPollEvents();//check events
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
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
