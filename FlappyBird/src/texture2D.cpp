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
	
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);
	
	//Texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilterMag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilterMin);

	stbi_set_flip_vertically_on_load(true);

	int nrChannels;
	unsigned char *data = stbi_load(filePath, &mWidth, &mHeight, &nrChannels, 0);
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

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, imageFormat, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, mId);
}

int Texture2D::GetWidth() const
{
	return mWidth;
}

int Texture2D::GetHeight() const
{
	return mHeight;
}

void Texture2D::Clear()
{
	glDeleteTextures(1, &mId);
}




