#include "animation.h"
#include <iostream>
#include "globals.h"

Animation::Animation(std::vector<Sprite*> &sprites, float animSpeed) : mSprites(sprites), mAnimSpeed(animSpeed) 
{
	mAnimTime = 1.0f / mAnimSpeed;
}

Animation::~Animation()
{
	Clear();
}

const Sprite* Animation::GetSprite(float dt)
{
	Sprite *sprite = mSprites[mIndex];
	mCount += dt;
	if (mCount >= mAnimTime)
	{
		mCount = 0.0f;
		mIndex++;
		if (mIndex >= mSprites.size())
		{
			mIndex = 0;
		}
	}
	return sprite;
}

void Animation::SetAnimSpeed(float animSpeed)
{
	mAnimSpeed = animSpeed;
}

void Animation::Reset()
{
	mIndex = 0;
	mCount = 0.0f;
}

void Animation::Clear()
{
	for (Sprite *sprite : mSprites)
	{
		DELETE_PTR(sprite);
	}
	mSprites.clear();
}