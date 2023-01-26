#pragma once

#ifndef Input_H
#define Input_H

#include "glad.h"
#include "glfw3.h"
#include "logger.h"
#include "KeyCode.hpp"

namespace Engine
{
	namespace Input {
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		class Input
		{
		public:

			static Input* Instance() {
				static Input* instance;

				if (!instance)
					instance = new Input();

				return instance;
			}

			/**
			 * @param window is a pointer to the renderer window pointer.
			 */
			void Init(GLFWwindow** rendererWindow);

			/*
				Important: call this method whenever the renderer window changes
			*/
			void UpdateWindow();

			//Must be called after polling events and the game logic or anyKey wont work as intended
			void processInput();

			//Returns true if any key has been presed
			bool anyKey();
			bool anyMouseKey();

			double getMousePositionX();
			double getMousePositionY();

			float getMouseDeltaX();
			float getMouseDeltaY();

			KeyCode getKey(KeyCode keyCode);

			KeyCode getMouseButton(KeyCode keyCode);

			bool isKeyPressed(KeyCode keyCode) {
				return getKey(keyCode) == KEY_STATE_PRESS;
			}
			bool isKeyReleased(KeyCode keyCode) {
				return getKey(keyCode) == KEY_STATE_RELEASE;
			}
			bool isKeyRepeated(KeyCode keyCode) {
				return getKey(keyCode) == KEY_STATE_REPEAT;
			}

			bool isMouseKeyPressed(KeyCode keyCode) {
				return getMouseButton(keyCode) == KEY_STATE_PRESS;
			}
			bool isMouseKeyReleased(KeyCode keyCode) {
				return getMouseButton(keyCode) == KEY_STATE_RELEASE;
			}
			bool isMouseKeyRepeated(KeyCode keyCode) {
				return getMouseButton(keyCode) == KEY_STATE_REPEAT;
			}

			void hideCursor() {
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}

			void showCursor() {
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}

			void invertMouse(bool xAxis, bool yAxis);

		private:
			//Variables-----------------------------------
			GLFWwindow** m_rendererWindow;
			GLFWwindow* m_window;


			//Methods-------------------------------------
			Input() {
				m_rendererWindow = nullptr;
				m_window = nullptr;
			}

			Input(Input const&) = delete;
			Input& operator=(Input const&) = delete;
		};
	}
}

#endif

