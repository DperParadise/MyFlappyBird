#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include <deque>
#include <utility>

class GameObject;
class SpriteRenderer;
typedef std::pair<GameObject*, GameObject*> ColumnPair;

class GameLevel final
{
public:
	GameLevel(int screenWidth, int screenHeight, float screenScaling, float minVerticalSeparation, float maxVerticalSeparation, float minVerticalPos, float maxVerticalPos, float shiftSpeedX, const SpriteRenderer *spriteRenderer);
	~GameLevel();
	void UpdateColumnsPosition(float dt);
	void DrawLevel(float dt) const;
	
private:
	std::deque<ColumnPair> mColumns;
	int mNumColumns = 20;
	int mScreenWidth;
	int mScreenHeight;
	float mScreenScaling;
	float mMinVerticalSeparation;
	float mMaxVerticalSeparation;
	float mMinVerticalPos;
	float mMaxVerticalPos;
	float mShiftSpeedX;
	const SpriteRenderer *mSpriteRenderer = nullptr;
	float mSpriteHeight;
	float mSpriteWidth;

	void Init();
	void ResetColumnPairPosition();
	void Clear();
};
#endif