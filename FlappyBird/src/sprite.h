#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "texture2D.h"

class Sprite final
{
public:
	Texture2D mTexture;
	float *mTexCoords;

	Sprite(const Texture2D &texture, int originX, int originY, int width, int height);
	~Sprite();
	int GetWidth() const;
	int GetHeight() const;

private:
	 const int mNumVertices = 12;
	 int mWidth;
	 int mHeight;
	 
};
#endif