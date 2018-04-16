#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>
#include "sprite.h"

class Animation final
{
public:
	Animation(std::vector<Sprite*> *sprites, float animSpeed);

	Sprite *GetSprite(float dt);
	void SetAnimSpeed(float animSpeed);
	void Reset();

private:
	std::vector<Sprite*> *mSprites;
	float count = 0.0f;
	int index = 0;

	//sprites per second
	float mAnimSpeed = 0.0f;

	//time between sprites
	float animTime;
};

#endif
