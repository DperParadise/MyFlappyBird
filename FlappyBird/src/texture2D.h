#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include <glad/glad.h>

class Texture2D final
{
public:

	Texture2D(const char *filePath);
	
	GLuint mWrapS;
	GLuint mWrapT;
	GLuint mFilterMin;
	GLuint mFilterMag;

	void Bind() const;
	int GetWidth() const;
	int GetHeight() const;
	void Clear();

private:
	GLuint id;
	int width;
	int height;
};






#endif