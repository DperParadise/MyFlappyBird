#ifndef __GUI_SCORE_H__
#define __GUI_SCORE_H__

#include <map>
#include "sprite.h"

class SpriteRenderer;
class GUIScore final
{
public:
	GUIScore(int screenWidth, int screenHeight, float screenScaling);
	~GUIScore();

	void Draw(unsigned int score, const SpriteRenderer *renderer) const;

private:
	std::map<char, Sprite*> mNumbers;
	float mScreenScaling;
	int mScreenWidth;
	int mScreenHeight;
	float mFactorScreenWidth;
	float mFactorScreenHeight;
	int mSpriteWidth;
	void Init();
	void Clear();
};

#endif
