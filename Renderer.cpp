#include "Renderer.h"
#include "logger.h"

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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

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
}


