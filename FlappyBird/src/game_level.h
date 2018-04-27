#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include <deque>
#include <utility>
#include <string>
#include <glm/glm.hpp>
#include <vector>

class ColumnGameObject;
class SpriteRenderer;
class Game;
class BirdGameObject;
typedef std::pair<ColumnGameObject*, ColumnGameObject*> ColumnPair;

class GameLevel final
{
public:
	std::deque<ColumnPair> mColumns;
	int mNumColumnPairs = 20;

	GameLevel(int screenWidth, int screenHeight, float screenScaling, int verticalSeparation, 
		int minVerticalPos, int maxVerticalPos, int passThreshold, 
		int shiftSpeedX, const SpriteRenderer *spriteRenderer, const std::string &flappyBirdSpriteAtlasName, 
		Game *game, const BirdGameObject *bird);
	~GameLevel();
	void UpdateColumnsPosition(float dt);
	void DrawLevel(float dt) const;
	float GetSpriteWidth() const;
	void Reset();
	void PrepareShake();
	void BeginShake();
	void EndShake();

private:
	
	int mScreenWidth;
	int mScreenHeight;
	float mScreenScaling;
	int mVerticalSeparation;
	int mMinVerticalPos;
	int mMaxVerticalPos;
	int mPassThreshold;
	float mShiftSpeedX;
	const SpriteRenderer *mSpriteRenderer = nullptr;
	float mSpriteHeight;
	float mSpriteWidth;
	const std::string &mFlappyBirdSpriteAtlasName;
	Game *mGame = nullptr;
	const BirdGameObject *mBird = nullptr;
	int mFactorHorizSeparation = 3;
	
	
	void Init();
	void ResetColumnPairPosition();
	void Clear();
	float ComputeXPos(float posLowerLeftX, float posLowerLeftY, float posUpperLeftY, float posLowerRightY, float posUpperRightY) const;
};
#endif