#include "Texture.h"

void Texture_Asset::loadFile() {

	log_printf(LOG_INFO, "Loading image: %s", filePath.c_str());

	glGenTextures(1, &m_ID);

	stbi_set_flip_vertically_on_load(m_flipYImage);

	m_image_data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_nChannels, 0);//Load image and fill width,height and nrChannels

	glBindTexture(GL_TEXTURE_2D, m_ID);
	parameters(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);//Default texture config. Wrap: repeat	Filter: linear

	//std::cout << m_nChannels << std::endl;

	if (m_image_data && m_image_data != NULL)
	{
		log_printf(LOG_DEBUG, "	Dimensions:	%ix%i\n	numChanels:	%i\n", m_width, m_height, m_nChannels);

		//Assign image to texture
		if (m_nChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_image_data);
		else if (m_nChannels == 4)
			if (m_format == GL_RGB)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image_data);

		//if (m_useMipmap) glGenerateMipmap(GL_TEXTURE_2D);//Automatically generate mipmaps
	}
	else {
		std::cout << "	ERROR::TEXTURE::FILE_NOT_SUCCESFULLY_READ: " << filePath << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);//Unbind texture

	stbi_image_free(m_image_data);//free resources
}
