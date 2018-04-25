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
	int mScore = 0;
	GUIScore *mGUIScore = nullptr;
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
	int mGetReadyX = 295;
	int	mGetReadyY = 59;
	int	mGetReadyWidth = 92;
	int	mGetReadyHeight = 25;
	float mGetReadyScreenFactorX = 0.5f;
	float mGetReadyScreenFactorY = 0.75f;
	int mInstructionsX = 292;
	int	mInstructionsY = 91;
	int	mInstructionsWidth = 57;
	int	mInstructionsHeight = 49;
	float mInstrucScreenFactorX = 0.5;
	float mInstrucScreenFactorY = 0.5f;
	int mTitleX = 350;
	int	mTitleY = 91;
	int mTitleWidth = 90;
	int mTitleHeight = 24;
	float mTitleScreenFactorX = 0.5f;
	float mTitleScreenFactorY = 0.75f;
	int mPlayButtonX = 354;
	int mPlayButtonY = 118;
	int mPlayButtonWidth = 52;
	int mPlayButtonHeight = 29;
	float mPlayBtnScreenFactorX = 0.5f;
	float mPlayBtnScreenFactorY = 0.25f;
	int mScoreBoardX = 3;
	int mScoreBoardY = 259;
	int	mScoreBoardWidth = 113;
	int	mScoreBoardHeight = 57;
	float mScoreBrdScreenFactorX = 0.5f;
	float mScoreBrdScreenFactorY = 0.5f;

	void LoadProperties();
	void DoCollissions();
	void CheckCollissions(BirdGameObject *bird, ColumnGameObject *column);
	void CheckCollissions(BirdGameObject *bird, int groundHeight);
	void Clear();
};

#endif