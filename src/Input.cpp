#include "Input.h"
#include <iostream>

namespace Engine
{
	namespace Input
	{
		bool m_anyKeyPressed = false;
		bool m_anyMouseKeyPressed = false;

		//Mouse
		double m_mousePosX = 0, m_mousePosY = 0;
		double m_lastMousePosX = 0, m_lastMousePosY = 0;

		bool m_invertMouseX, m_invertMouseY;

		//Update mouse info
		void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

		}
		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

			m_anyMouseKeyPressed = true;
		}
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

		}
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
			
			//log_printf(log_level_e::LOG_INFO, "Pressed key %i\n",key);

			m_anyKeyPressed = true;
		}

		void Input::Init(GLFWwindow** rendererWindow) {
			
			m_rendererWindow = rendererWindow;
			m_window = *m_rendererWindow;
			UpdateWindow();

			//glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);//the key will maintain its state untill polled with glfwGetKey

			log_message(log_level_e::LOG_INFO, "	Initialized Input");
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
			glfwSetKeyCallback(m_window, key_callback);
		}
		
		void Input::processInput() {


			//UpdateMouse movement
			double xpos, ypos;
			glfwGetCursorPos(m_window, &xpos, &ypos);
			//Horizontal axis
			m_lastMousePosX = m_mousePosX;
			m_mousePosX = xpos;
			//Vertical Axis
			m_lastMousePosY = m_mousePosY;
			m_mousePosY = ypos;

			//Reset anykey pressed
			m_anyKeyPressed = false;
			m_anyMouseKeyPressed = false;
		}

		bool Input::anyKey() { return m_anyKeyPressed; }
		bool Input::anyMouseKey() { return m_anyMouseKeyPressed; }

		double Input::getMousePositionX() { return m_mousePosX; }
		double Input::getMousePositionY() { return m_mousePosY; }

		float Input::getMouseDeltaX() { 
			return (float)((m_invertMouseX) ? -1 : 1) * (m_mousePosX - m_lastMousePosX);
		}
		float Input::getMouseDeltaY() {
			return (float)((m_invertMouseY) ? -1 : 1) * (m_lastMousePosY - m_mousePosY);
		}

		void Input::invertMouse(bool xAxis, bool yAxis) {
			m_invertMouseX = xAxis;
			m_invertMouseY = yAxis;
		}


		KeyCode Input::getKey(KeyCode keyCode) {
			return glfwGetKey(m_window, keyCode);
		}

		KeyCode Input::getMouseButton(KeyCode keyCode) {
			return glfwGetMouseButton(m_window, keyCode);
		}
	}
}


