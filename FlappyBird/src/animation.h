#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>
#include "sprite.h"

class Animation final
{
public:
	std::vector<Sprite*> mSprites;
	Animation(std::vector<Sprite*> &sprites, float animSpeed);
	~Animation();

	const Sprite *GetSprite(float dt);
	void Reset();
	void Clear();

private:
	float mCount = 0.0f;
	int mIndex = 0;

	//sprites per second
	float mAnimSpeed = 0.0f;

	//time between sprites
	float mAnimTime;
};

#endif
