#include "texture2D.h"
#include <iostream>
#include "stb_image.h"


Texture2D::Texture2D(const char *filePath) : mWrapS(GL_REPEAT), mWrapT(GL_REPEAT), mFilterMin(GL_LINEAR), mFilterMag(GL_LINEAR)
{
	if (filePath == nullptr)
	{
		std::cout << "error : filePath null pointer" << std::endl;
		return;
	}
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	//Texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilterMag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilterMin);

	int nrChannels;
	unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);
	if (data == NULL)
	{
		std::cout << "error loading the texture:" << filePath << std::endl;
	}
	
	GLuint internalFormat;
	GLuint imageFormat;

	switch (nrChannels)
	{
	case 3:
		internalFormat = GL_RGB8;
		imageFormat = GL_RGB;
		break;
	case 4:
		internalFormat = GL_RGBA8;
		imageFormat = GL_RGBA;
		break;
	default:
		std::cout << "Wrong number of channels in image: " << nrChannels << std::endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}

GLuint Texture2D::GetId() const
{
	return id;
}

int Texture2D::GetWidth() const
{
	return width;
}

int Texture2D::GetHeight() const
{
	return height;
}




