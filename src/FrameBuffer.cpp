#include "FrameBuffer.h"

#include "logger.h"

namespace Engine
{
	FrameBuffer::FrameBuffer(int width, int height, int frameTypeFlag, int atachment, GLint colorInternalFormat) {

		m_width = width;
		m_height = height;
		m_frameType = frameTypeFlag;

		//framebuffer
		glGenFramebuffers(1, &m_FramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		if (!glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
			error_OnLoad = true;
			log_error("ERROR::FRAMEBUFFER::FAILED_TO_CREATE_FRAMEBUFFER");

			unbind();
			return;
		}

		//Color texture
		if (frameTypeFlag & FrameType_Color) {
			glGenTextures(1, &m_ColorID);

			glBindTexture(GL_TEXTURE_2D, m_ColorID);

			// Give an empty image to OpenGL ( the last "0" )
			glTexImage2D(GL_TEXTURE_2D, 0, colorInternalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			//set texture as color atachemnt 
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + atachment, GL_TEXTURE_2D, m_ColorID, 0);
		}

		//Depth texture
		if (frameTypeFlag & FrameType_Depth) {
			glGenTextures(1, &m_DepthID);

			glBindTexture(GL_TEXTURE_2D, m_DepthID);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthID, 0);
		}

		// Set the list of draw buffers.
		GLenum DrawBuffers[2];

		switch (frameTypeFlag)
		{
		case FrameType_Color:
			DrawBuffers[0] = GL_COLOR_ATTACHMENT0 + atachment;
			glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
			break;
		case FrameType_Depth:
			DrawBuffers[0] = GL_DEPTH_ATTACHMENT;
			glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
			break;
		case FrameType_Color | FrameType_Depth:
			DrawBuffers[0] = GL_COLOR_ATTACHMENT0 + atachment;
			DrawBuffers[1] = GL_DEPTH_ATTACHMENT;
			glDrawBuffers(2, DrawBuffers); // "2" is the size of DrawBuffers
			break;
		default:
			error_OnLoad = true;
			log_error("ERROR::FRAMEBUFFER::INVALID_FRAME_TYPE");

			unbind();
			return;
			break;
		}

		error_OnLoad = false;
		unbind();
	}
	void FrameBuffer::bind(bool changeViewport)
	{
		if (error_OnLoad) {
			log_error("ERROR::FRAMEBUFFER::FAILED_TO_BIND_FRAMEBUFFER");
			return;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

		if(changeViewport)
			glViewport(0, 0, m_width, m_height);
	}
	void FrameBuffer::bindColorTexture(int textureUnit) const
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, m_ColorID);
	}
	void FrameBuffer::bindDepthTexture(int textureUnit) const
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, m_DepthID);
	}
}

