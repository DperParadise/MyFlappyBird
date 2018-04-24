#ifndef __GAME_H__
#define __GAME_H__

#include "game_level.h"

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
	int mScore = 0;
	int mColumnIndex = 0;

	void DoCollissions();
	void CheckCollissions(BirdGameObject *bird, ColumnGameObject *column);
	void CheckCollissions(BirdGameObject *bird, int groundHeight);
	void Clear();
};

#endif