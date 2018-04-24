#ifndef __GAME_H__
#define __GAME_H__

#include "game_level.h"
#include <string>
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
	std::string mFlappyBirdSpriteAtlasName; //default: FlappyBirdSpriteAtlas
	std::string mFlappyBirdSpriteAtlasPath; //default: resources/textures/flappy_bird_sprite_sheet.png
	std::string mForegroundTexName; //default: Foreground
	std::string mForegroundTexPath; //default: resources/textures/foreground.png
	std::string mGameShaderName; //default: GameShader
	std::string mVertexShaderPath; //default: resources/shaders/quad.vs
	std::string mFragmentShaderPath; //default = resources/shaders/quad.fs
	int mBirdSpriteWidth = 17;
	int mBirdSpriteHeight = 12;
	int mBirdSpriteY = 491;
	int mBirdSprite1X = 3;
	int mBirdSprite2X = 31;
	int mBirdSprite3X = 59;
	float mBirdAnimSpeed = 10.0f;
	glm::vec2 mStartPos; //(0.33 * mScreenWidth , 0.5 * mScreenHeight)
	float mStartRot = 30.0f;
	float mWorldAcceleration = -2000.0f;
	int mColumnVerticalSeparation = 64;
	int mMinVerticalPos = -104;
	int mMaxVerticalPos = 24;
	int mPassThreshold = 36;
	int mShiftSpeed = -100;
	int mBGroundSpriteOrigin = 0;
	int mBGroundSpriteWidth = 144;
	int mBGroundSpriteHeight = 256;
	int mFGroundSpriteOrigin = 0;
	int mFGroundSpriteWidth = 168;
	int mFGroundSpriteHeight = 56;

	void LoadProperties();
	void DoCollissions();
	void CheckCollissions(BirdGameObject *bird, ColumnGameObject *column);
	void CheckCollissions(BirdGameObject *bird, int groundHeight);
	void Clear();
};

#endif