#ifndef __GUI_SCORE_H__
#define __GUI_SCORE_H__

#include <map>
#include "sprite.h"
#include <glm/glm.hpp>

class Sprite;
class SpriteRenderer;
class Texture2D;
class GUIScore final
{
public:
	GUIScore(int screenWidth, int screenHeight, float screenScaling);
	~GUIScore();
	void DrawBigScoreNumbers(unsigned int score, const SpriteRenderer *renderer) const;
	void DrawScoreBoard(unsigned int score, const SpriteRenderer *renderer) const;
	void SaveMaxScore(unsigned int maxScore);
	unsigned int GetBestScore() const;

private:
	unsigned int mBestScore;
	std::map<char, Sprite*> mBigNumbers;
	std::map<char, Sprite*> mSmallNumbers;
	float mScreenScaling;
	int mScreenWidth;
	int mScreenHeight;
	float mFactorScreenWidthBig;
	float mFactorScreenHeightBig;

	//offsets in the score board to position score numbers
	int mSBEdgeOffsetWidth = 11; 
	int mSBEdgeOffsetHeight = 23;
	int mBestEdgeOffsetWidth = 11;
	int mBestEdgeOffsetHeight = 44;

	//score board multiplicative factors
	float mSBFactorScreenWidth;
	float mSBFactorScreenHeight;

	Sprite *mScoreBoard = nullptr;

	void Init();
	void DrawSmallScoreNumbers(const Sprite *scoreBoard, const glm::vec2 &scoreBoardPos, int score, const SpriteRenderer *renderer, bool isBestScore) const;
	void LoadBestScore();
	
	void Clear();
};

#endif
