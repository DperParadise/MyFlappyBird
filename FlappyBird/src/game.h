#ifndef __GAME_H__
#define __GAME_H__

#include "game_level.h"
#include <glm/glm.hpp>

enum class GameState
{
	MENU,
	ACTIVE,
	DEAD,
};
class BirdGameObject;
class ColumnGameObject;
class GameLevel;
class SpriteRenderer;
class Sprite;
class GUIScore;
class Game final
{
public:
	GameState mGameState;
	bool mKeys[1024];
	bool mKeysProcessed[1024];

	Game(int screenWidth, int screenHeight, float mScreenScaling);
	~Game();
	void Init();
	void ProcessInput();
	void Update(float dt);
	void Render(float dt);
	void ComputeScore();
	
private:
	int mScreenWidth, mScreenHeight;
	float mScreenScaling;
	GameLevel *mLevel = nullptr;
	SpriteRenderer * mRenderer = nullptr;
	BirdGameObject *mFlappyBird = nullptr;
	Sprite *mBackground = nullptr;
	Sprite *mForeground = nullptr;
	Sprite *mGetReady = nullptr;
	Sprite *mInstructions = nullptr;
	Sprite *mTitle = nullptr;
	Sprite *mPlayButton = nullptr;
	Sprite *mScoreBoard = nullptr;
	Sprite *mGameOver = nullptr;
	int mScore = 0;
	GUIScore *mGUIScore = nullptr;
	int mColumnIndex = 0;
	int mShiftSpeed = -100;
	float mGetReadyScreenFactorX = 0.5f;
	float mGetReadyScreenFactorY = 0.75f;
	float mInstrucScreenFactorX = 0.5;
	float mInstrucScreenFactorY = 0.5f;
	float mTitleScreenFactorX = 0.5f;
	float mTitleScreenFactorY = 0.75f;
	float mPlayBtnScreenFactorX = 0.5f;
	float mPlayBtnScreenFactorY = 0.25f;
	float mGameOverScreenFactorX = 0.5f;
	float mGameOverScreenFactorY = 0.75f;

	void LoadProperties();
	void DoCollissions();
	void CheckCollissions(BirdGameObject *bird, ColumnGameObject *column);
	void CheckCollissions(BirdGameObject *bird, int groundHeight);
	void Clear();
};

#endif