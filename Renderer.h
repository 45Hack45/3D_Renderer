#pragma once

#ifndef RENDERER_H_
#define RENDERER_H_

#include "glad.h"
#include "glfw3.h"

#include "logger.h"

#include "Color.h"

namespace Engine
{
	class Renderer {
	public:
		//Methods-------------------------------------
		static Renderer* Instance() {
			static Renderer* instance;

			if (!instance)
				instance = new Renderer();

			return instance;
		}

		int Init(const char* window_title = "Engine", bool fullscreen = false);

		GLFWwindow* getWindow()const { return m_window; }

		void ClearScreen(const Color& color) {
			//Clearing screen
			glClearColor(color.color_vect.r, color.color_vect.g, color.color_vect.b, 1.0f);
			//glClearColor(.0f, 0.f, 0.f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		int getWindowSizeX()const;
		int getWindowSizeY()const;
		float getWindowAspectRatio()const;

		//Variables-----------------------------------
		GLFWwindow* m_window;//Window

	private:
		//Variables-----------------------------------
		GLFWmonitor* m_primaryMonitor;//Fullscreen monitor

		bool m_fullscreen_mode;

		//Methods-------------------------------------
		Renderer() {
			m_primaryMonitor = glfwGetPrimaryMonitor();
			
			if (!m_primaryMonitor)
			{
				log_error("ERROR::RENDERER::FAILED_TO_GET_PRIMARY_MONITOR	Won't be able to use fullscreen mode\n");
			}
		}

		Renderer(Renderer const&) = delete;
		Renderer& operator=(Renderer const&) = delete;

		void setWindowSizeX(int value);
		void setWindowSizeY(int value);
	};
}

#endif

