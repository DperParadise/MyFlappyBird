#ifndef __GAME_H__
#define __GAME_H__

#include "game_level.h"
#include <glm/glm.hpp>

enum class GameState
{
	INITIAL_SCREEN,
	INSTRUCTIONS,
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
	GameState mGameState = GameState::INITIAL_SCREEN;
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
	float mFactorStartPosX = 0.25f;
	float mFactorStartPosY = 0.5f;
	int mShiftSpeed = -100;
	float mFactorGetReadyScreenX = 0.5f;
	float mFactorGetReadyScreenY = 0.65f;
	float mFactorInstrucScreenX = 0.5;
	float mFactorInstrucScreenY = 0.35f;
	float mFactorTitleScreenX = 0.5f;
	float mFactorTitleScreenY = 0.75f;
	float mFactorPlayBtnScreenX = 0.5f;
	float mFactorPlayBtnScreenY = 0.25f;
	float mFactorGameOverScreenX = 0.5f;
	float mFactorGameOverScreenY = 0.75f;

	void LoadProperties();
	void DoCollissions();
	void CheckCollissions(BirdGameObject *bird, ColumnGameObject *column);
	void CheckCollissions(BirdGameObject *bird, int groundHeight);
	void Clear();
};

#endif