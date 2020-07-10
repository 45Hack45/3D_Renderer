#include "Input.h"

namespace Engine
{
	namespace Input
	{
		void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

		}
		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

		}
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

		}

		void Input::Init(GLFWwindow** rendererWindow) {
			m_rendererWindow = rendererWindow;
			UpdateWindow();
		}

		void Input::UpdateWindow() {

			//Unbind callbacks
			glfwSetCursorPosCallback(m_window, NULL);
			glfwSetScrollCallback(m_window, NULL);
			glfwSetMouseButtonCallback(m_window, NULL);

			//Binding callbacks to the new window
			m_window = *m_rendererWindow;

			glfwSetCursorPosCallback(m_window, mouse_callback);
			glfwSetScrollCallback(m_window, scroll_callback);
			glfwSetMouseButtonCallback(m_window, mouse_button_callback);
		}

		void processInput() {

		}
	}
}