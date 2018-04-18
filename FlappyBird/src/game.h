#ifndef __GAME_H__
#define __GAME_H__

#include "game_level.h"

enum class GameState
{
	MENU,
	ACTIVE,
	DEAD,
};
class GameObject;
class GameLevel;
class SpriteRenderer;
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
	
private:
	int mScreenWidth, mScreenHeight;
	float mScreenScaling;
	GameLevel *mLevel = nullptr;
	SpriteRenderer * mRenderer = nullptr;
	GameObject *mFlappyBird = nullptr;
	void DoCollissions();
	void CheckCollissions(GameObject *one, GameObject *two);
	void Clear();
};

#endif