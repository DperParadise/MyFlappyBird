#include "animation.h"
#include <iostream>

Animation::Animation(std::vector<Sprite*> *sprites, float animSpeed) : mSprites(sprites), mAnimSpeed(animSpeed) 
{
	animTime = 1.0f / mAnimSpeed;
}

Sprite* Animation::GetSprite(float dt)
{
	Sprite *sprite = mSprites->at(index);
	count += dt;
	if (count >= animTime)
	{
		count = 0.0f;
		index++;
		if (index >= mSprites->size())
		{
			index = 0;
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
	index = 0;
	count = 0.0f;
}