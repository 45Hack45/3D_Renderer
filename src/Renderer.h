#pragma once

#ifndef RENDERER_H_
#define RENDERER_H_

#include <vector>

#include "glad.h"
#include "glfw3.h"

#include "logger.h"
#include "Color.h"

#include "FrameBuffer.h"

namespace Engine
{
	class IRenderPass
	{
	public:
		static const unsigned int RenderPassOrder_Opaque =		4;
		static const unsigned int RenderPassOrder_Transparent = 8;
		static const unsigned int RenderPassOrder_FullScreen =	12;

		//Draw.
		//Optional input framebuffer
		//Optional output framebuffer
		virtual const FrameBuffer* RenderPass(const FrameBuffer* in_fbo) = 0;

		virtual void RenderPassDebugGUI() {};

		//Returns render pass render order. Lower value are rendered first
		virtual unsigned int getRenderOrder() const = 0;
	};

	class Renderer {
	public:
		//Methods-------------------------------------
		static Renderer* Instance() {
			static Renderer* instance;

			if (!instance) {
				instance = new Renderer();
			}

			return instance;
		}

		int Init(const char* window_title = "Engine", bool fullscreen = false);
		void InitRenderFBO() {
			//Init render framebuffer
			fbo = new FrameBuffer(getWindowSizeX(), getWindowSizeY(), FrameBuffer::FrameType_Depth | FrameBuffer::FrameType_Color, 0, GL_RGB);
		};

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

		//Does all the render passes
		void Render();

		void addRenderPass(IRenderPass* renderPass);

		//draws a full screen quad with a texture.
		//if useFullScreenQuadShader is false will use the previously binded shader (useful for sending shader info for postprocessing effects)
		static void drawFullScreenQuad(unsigned int textureID, unsigned int textureAtachment = 0, bool useFullScreenQuadShader = true);

		//Variables-----------------------------------
		GLFWwindow* m_window = nullptr;//Window
		FrameBuffer* fbo;//Frame buffer object

	private:
		//Variables-----------------------------------
		GLFWmonitor* m_primaryMonitor;//Fullscreen monitor

		bool m_fullscreen_mode = false;

		std::vector<IRenderPass*> renderPasses = std::vector<IRenderPass*>();

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

