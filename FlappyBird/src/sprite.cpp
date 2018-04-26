#include "sprite.h"
#include "texture2D.h"
#include "globals.h"

Sprite::Sprite(const Texture2D &texture, 
	int originX, int originY, int width, int height) : mTexture(texture), mWidth(width), mHeight(height)
{
	//Adptation from origin at the top left (image coord system) to origin at the bottom left (OpenGL coord system)
	mTexCoords = new float[mNumVertices] {	
		(float)(originX) / mTexture.GetWidth(),	(float)(mTexture.GetHeight() - (originY + height)) / mTexture.GetHeight(),
		(float)(originX + width) / mTexture.GetWidth(),	(float)(mTexture.GetHeight() - (originY + height)) / mTexture.GetHeight(),
		(float)(originX) / mTexture.GetWidth(),	(float)(mTexture.GetHeight() - originY)/ mTexture.GetHeight(),
		(float)(originX + width) / mTexture.GetWidth(), (float)(mTexture.GetHeight() - (originY + height)) / mTexture.GetHeight(),
		(float)(originX + width) / mTexture.GetWidth(), (float)(mTexture.GetHeight() - originY) / mTexture.GetHeight(),
		(float)(originX) / mTexture.GetWidth(), (float)(mTexture.GetHeight() - originY) / mTexture.GetHeight()
	};
}

Sprite::~Sprite()
{
	DELETE_PTR_ARRAY(mTexCoords);
}

int Sprite::GetWidth() const
{
	return mWidth;
}

int Sprite::GetHeight() const
{
	return mHeight;
}

