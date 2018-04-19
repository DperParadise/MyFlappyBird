#include "sprite.h"
#include "texture2D.h"
#include "globals.h"

Sprite::Sprite(const Texture2D &texture, 
	int originX, int originY, int width, int height) : mTexture(texture), mWidth(width), mHeight(height)
{
	mTexCoords = new float[mNumVertices]{	(float)(originX) / (float)mTexture.GetWidth(), (float)(originY + height) / (float)mTexture.GetHeight(),
										(float)(originX + width) / (float)mTexture.GetWidth(), (float)(originY + height) / (float)mTexture.GetHeight(),
										(float)(originX) / (float) mTexture.GetWidth(), (float)originY / (float) mTexture.GetHeight(),
										(float)(originX + width) / (float)mTexture.GetWidth(), (float)(originY + height) / mTexture.GetHeight(),
										(float)(originX + width) / mTexture.GetWidth(), (float)(originY) / mTexture.GetHeight(),
										(float)(originX) / mTexture.GetWidth(), (float)(originY) / mTexture.GetHeight()
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

