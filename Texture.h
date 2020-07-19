#ifndef Texture_H
#define Texture_H

#include <iostream>

#include "glad.h"

#include "logger.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture
{
public:
	Texture(const char* imagePath, GLint imageFormat = GL_RGB,bool flipYImage = true, bool useMipmap = true) {

		glGenTextures(1, &m_ID);

		stbi_set_flip_vertically_on_load(flipYImage);

		m_image_data = stbi_load(imagePath, &m_width, &m_height, &m_nChannels, 0);//Load image and fill width,height and nrChannels

		log_printf(LOG_INFO, "Loading image: %s", imagePath);

		glBindTexture(GL_TEXTURE_2D, m_ID);
		parameters(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);//Default texture config. Wrap: repeat	Filter: linear

		//std::cout << m_nChannels << std::endl;

		if (m_image_data)
		{
			log_printf(LOG_DEBUG, "	Dimensions:	%ix%i\n	numChanels:	%i", m_width, m_height, m_nChannels);

			//Assign image to texture
			if(m_nChannels == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_image_data);
			else if (m_nChannels == 4)
				if(imageFormat == GL_RGB)
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image_data);

			if(useMipmap) glGenerateMipmap(GL_TEXTURE_2D);//Automatically generate mipmaps
		}else {
			std::cout << "	ERROR::TEXTURE::FILE_NOT_SUCCESFULLY_READ: " << imagePath << std::endl;
		}

		stbi_image_free(m_image_data);//free resources
	}

	void parameters(GLint hWrap, GLint vWrap, GLint hFilter, GLint vFilter) {
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, hWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, hFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, vFilter);
	}

	unsigned int ID()const { return m_ID; }
	int width()const { return m_width; }
	int height()const { return m_height; }
	int nChanels()const { return m_nChannels; }

	void bind(const Shader& shader, const char uniformName) {
	
	}

private:
	unsigned int m_ID;
	int m_width, m_height, m_nChannels;
	unsigned char* m_image_data;
};

#endif

