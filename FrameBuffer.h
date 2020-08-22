#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "glad.h";

namespace Engine
{
	class FrameBuffer
	{
	public:
		//Methods-------------------------------------
		FrameBuffer(int width, int height, int frameTypeFlag = FrameType_Color, int atachment = 0, GLint colorInternalFormat = GL_RGB);

		~FrameBuffer()
		{
			glDeleteFramebuffers(1, &m_FramebufferID);
		}

		//Binds the frame buffer
		//if changeViewport is true the viewport will be set to the framebuffer dimensions
		void bind(bool changeViewport = false);
		void bindColorTexture(int textureUnit = 0);
		void bindDepthTexture(int textureUnit = 1);


		//Unbinds any binded framebuffer and sets the screen as the render target
		static void unbind(){
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		GLuint framebufferID()const { return m_FramebufferID; }
		GLuint colorTextureID()const { return m_ColorID; }
		GLuint depthTextureID()const { return m_DepthID; }

		//Variables-----------------------------------
		static const int FrameType_Color = 1, FrameType_Depth = 2;

	private:
		//Variables-----------------------------------
		GLuint m_FramebufferID = 0;
		GLuint m_ColorID = 0;
		GLuint m_DepthID = 0;

		int m_frameType;
		int m_width, m_height;

		bool error_OnLoad;

		//Methods-------------------------------------
	};
}

#endif

