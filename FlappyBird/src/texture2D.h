#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include <glad/glad.h>

class Texture2D final
{
public:

	/*Creates a texture2D
	@param wrapS : wrapping mode in S axis : GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
	@param wrapT : wrapping mode in T axis : GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
	@param filterMin : minification filter (screen pixels < texture pixels) : GL_LINEAR, GL_NEAREST, etc
	@param filterMag : magnification filter (screen pixels > texture pixels) : GL_LINEAR, GL_NEAREST
	*/
	Texture2D(	GLuint wrapS,
				GLuint wrapT,
				GLuint filterMin,
				GLuint filterMag,
				const char *filePath);

	
	void Bind() const;
	GLuint GetId() const;
	int GetWidth() const;
	int GetHeight() const;

private:
	GLuint id;
	int width;
	int height;
};






#endif