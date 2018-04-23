#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include <deque>
#include <utility>

class ColumnGameObject;
class SpriteRenderer;
typedef std::pair<ColumnGameObject*, ColumnGameObject*> ColumnPair;

class GameLevel final
{
public:
	std::deque<ColumnPair> mColumns;
	int mNumColumnPairs = 20;

	GameLevel(int screenWidth, int screenHeight, float screenScaling, float minVerticalSeparation, float maxVerticalSeparation, float minVerticalPos, float maxVerticalPos, float passThreshold, float shiftSpeedX, const SpriteRenderer *spriteRenderer);
	~GameLevel();
	void UpdateColumnsPosition(float dt);
	void DrawLevel(float dt) const;
	
private:
	
	int mScreenWidth;
	int mScreenHeight;
	float mScreenScaling;
	float mMinVerticalSeparation;
	float mMaxVerticalSeparation;
	float mMinVerticalPos;
	float mMaxVerticalPos;
	float mPassThreshold;
	float mShiftSpeedX;
	const SpriteRenderer *mSpriteRenderer = nullptr;
	float mSpriteHeight;
	float mSpriteWidth;

	void Init();
	void ResetColumnPairPosition();
	void Clear();
	float ComputeXPos(float posLowerLeftX, float posLowerLeftY, float posUpperLeftY, float posLowerRightY, float posUpperRightY) const;
};
#endif