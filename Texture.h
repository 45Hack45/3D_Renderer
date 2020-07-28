#pragma once
#ifndef Texture_H
#define Texture_H

#include <iostream>

#include "glad.h"

#include "logger.h"
#include "Shader.h"
#include "Asset.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture: Engine::Asset
{
public:
	Texture(const char* imagePath, GLint imageFormat = GL_RGB,bool flipYImage = true, bool useMipmap = true):Asset(imagePath, imagePath) {
		m_flipYImage = flipYImage;
		m_format = imageFormat;
		m_useMipmap = useMipmap;

		loadFile();
	}

	void loadFile();

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
	GLint format()const { return m_format; }

	bool imageYFliped()const { return m_flipYImage; }

	//Binds the texture to a textureUnit of a binded shader.
	//Alert: Make shure to bid shader befor calling this method
	void bind(int textureUnit = 0) {
		glActiveTexture(GL_TEXTURE0 + textureUnit); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

private:
	unsigned int m_ID;
	int m_width, m_height, m_nChannels;
	unsigned char* m_image_data;

	bool m_flipYImage;
	GLint m_format;
	bool m_useMipmap;
};

#endif

