#include "Renderer.h"
#include "logger.h"
#include "ShaderManager.hpp"

namespace Engine
{

	int m_window_size_X = 800*2, m_window_size_Y = 450*2;
	bool render2fullWindow = true;

	//Resize viewport
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		m_window_size_X = width;
		m_window_size_Y = height;
		glViewport(0, 0, m_window_size_X, m_window_size_Y);
	}

	int Renderer::Init(const char* window_title,bool fullscreen) {
		log_message(log_level_e::LOG_DEBUG, "	Initializing Renderer");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_fullscreen_mode = fullscreen;

		m_window = glfwCreateWindow(m_window_size_X, m_window_size_Y, window_title, (m_fullscreen_mode) ? m_primaryMonitor : NULL, NULL);

		if (m_window == NULL)
		{
			log_error("ERROR::RENDERER::GLFW::WINDOW::FAILED_TO_CREATE_GLFW_WINDOW\n");
			glfwTerminate();
			return -1;
		}
		//Seting window
		glfwMakeContextCurrent(m_window);

		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);//Telling glad to call framebuffer_size_callback on resize window

		glfwSetWindowAspectRatio(m_window, 800, 450);

		log_message(log_level_e::LOG_INFO, "	Renderer Initialized");
		return 0;
	}

	int Renderer::getWindowSizeX()const {
		return m_window_size_X;
	}
	int Renderer::getWindowSizeY()const {
		return m_window_size_Y;
	}

	void Renderer::setWindowSizeX(int value) {
		m_window_size_X = value;
	}
	void Renderer::setWindowSizeY(int value) {
		m_window_size_Y = value;
	}

	float Renderer::getWindowAspectRatio()const {
		return m_window_size_X / (1 + m_window_size_Y);
	}

	bool renderPassSortPred(const IRenderPass* a, const IRenderPass* b) {
		return a->getRenderOrder() < b->getRenderOrder();
	}

	void Renderer::Render()
	{
		const FrameBuffer* aux = nullptr;

		for (int i = 0; i < renderPasses.size(); i++)
		{
			aux = renderPasses[i]->RenderPass(aux);
		}

		

		for (int i = 0; i < renderPasses.size(); i++)
		{
			renderPasses[i]->RenderPassDebugGUI();
		}
	}

	void Renderer::addRenderPass(IRenderPass* renderPass)
	{
		renderPasses.push_back(renderPass);

		//Sort in ascending order
		std::sort(renderPasses.begin(), renderPasses.end(), renderPassSortPred);
	}

	void Renderer::drawFullScreenQuad(unsigned int textureID, unsigned int textureAtachment, bool useFullScreenQuadShader) {

		static float vertices[] = {
			//Position		
			 1,  1, 0.0f,	1, 1,	// top right
			 1, -1, 0.0f,	1, 0,	// bottom right
			-1, -1, 0.0f,	0, 0,	// bottom left
			-1,  1, 0.0f,	0, 1	// top left 
		};
		static unsigned int indices[] = {  // note that we start from 0!
			2,0,3,
			2,1,0
		};
		static unsigned int VBO, VAO, EBO;

		static bool initialized = false;

		if (!initialized) {
			//create buffers
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
		}


		if (useFullScreenQuadShader) {
			Shader* sh = ShaderManager::Instance()->getShader("FullScreenQuadShader");
			sh->bind();
		}

		//bind texture
		glActiveTexture(GL_TEXTURE0 + textureAtachment); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, textureID);


		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glDeleteVertexArrays(1, &VAO);
		//glDeleteBuffers(1, &VBO);
		//glDeleteBuffers(1, &EBO);
	}
}


