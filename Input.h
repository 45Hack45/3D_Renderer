#pragma once

#ifndef Input_H
#define Input_H

#include "glad.h"
#include "glfw3.h"

namespace Engine
{
	namespace Input {
		void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		class Input
		{
		public:

			Input* Instance() {
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

			void processInput();

		private:
			//Variables-----------------------------------
			GLFWwindow** m_rendererWindow;
			GLFWwindow* m_window;

			//Methods-------------------------------------
			Input() {

			}

			Input(Input const&) = delete;
			Input& operator=(Input const&) = delete;
		};
	}
}

#endif