#include "game.h"
#include "resource_manager.h"
#include "bird_game_object.h"
#include "game_level.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "sprite_renderer.h"
#include "sprite.h"
#include "animation.h"
#include <GLFW/glfw3.h>
#include "globals.h"
#include "column_game_object.h"
#include "gui_score.h"
#include <string>

Game::Game(int screenWidth, int screenHeight, float screenScaling) : mScreenWidth(screenWidth), mScreenHeight(screenHeight), mScreenScaling(screenScaling) {}

Game::~Game()
{
	Clear();
}

void Game::Init()
{
	memset(mKeys, 0, 1024 * sizeof(bool));
	memset(mKeysProcessed, 0, 1024 * sizeof(bool));

	ResourceManager::LoadProperties(PROPERTIES_PATH);
	LoadProperties();

	std::string flappyBirdSpriteAtlasName = ResourceManager::GetPropString("Game.FlappyBirdSpriteAtlasName");
	std::string flappyBirdSpriteAtlasPath = ResourceManager::GetPropString("Game.FlappyBirdSpriteAtlasPath");
	ResourceManager::LoadTexture(flappyBirdSpriteAtlasName, flappyBirdSpriteAtlasPath);

	std::string foregroundTexName = ResourceManager::GetPropString("Game.ForegroundTexName");
	std::string foregroundTexPath = ResourceManager::GetPropString("Game.ForegroundTexPath");
	ResourceManager::LoadTexture(foregroundTexName, foregroundTexPath);

	std::string gameShaderName = ResourceManager::GetPropString("Game.GameShaderName");
	std::string vertexShaderPath = ResourceManager::GetPropString("Game.VertexShaderPath");
	std::string fragmentShaderPath = ResourceManager::GetPropString("Game.FragmentShaderPath");
	ResourceManager::LoadShader(gameShaderName, vertexShaderPath, fragmentShaderPath);
	glm::mat4 projection;
	projection = glm::ortho(0.0f, float(mScreenWidth), 0.0f, float(mScreenHeight), -1.0f, 1.0f);
	ResourceManager::GetShader(gameShaderName).SetMatrix4("projection", projection);
	ResourceManager::GetShader(gameShaderName).SetInteger("textureSampler", 0);
	
	mRenderer = new SpriteRenderer(ResourceManager::GetShader(gameShaderName), mScreenScaling);
																							
	std::vector<Sprite*> birdSprites;
	int birdSpriteWidth = ResourceManager::GetPropInt("Game.BirdSpriteWidth");
	int birdSpriteHeight = ResourceManager::GetPropInt("Game.BirdSpriteHeight");

	int birdSpriteY = ResourceManager::GetPropInt("Game.BirdSpriteY");
	int birdSprite1X = ResourceManager::GetPropInt("Game.BirdSprite1X");
	int birdSprite2X = ResourceManager::GetPropInt("Game.BirdSprite2X");
	int birdSprite3X = ResourceManager::GetPropInt("Game.BirdSprite3X");
	birdSprites.push_back(new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), birdSprite1X, birdSpriteY, birdSpriteWidth, birdSpriteHeight));
	birdSprites.push_back(new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), birdSprite2X, birdSpriteY, birdSpriteWidth, birdSpriteHeight));
	birdSprites.push_back(new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), birdSprite3X, birdSpriteY, birdSpriteWidth, birdSpriteHeight));
	
	float startPosX = ResourceManager::GetPropFloat("Game.StartPosX");
	float startPosY = ResourceManager::GetPropFloat("Game.StartPosY");
	float startRot = ResourceManager::GetPropFloat("Game.StartRot");
	float worldGravity = ResourceManager::GetPropFloat("Game.WorldGravity");
	float birdAnimSpeed = ResourceManager::GetPropFloat("Game.BirdAnimSpeed");
	Animation *flyAnimation = new Animation(birdSprites, birdAnimSpeed);
	mFlappyBird = new BirdGameObject(glm::vec2(startPosX, startPosY), startRot, glm::vec2(0.0f, 0.0f), worldGravity, flyAnimation);
		
	int columnVerticalSeparation = ResourceManager::GetPropInt("Game.ColumnVerticalSeparation");
	int minVerticalPos = ResourceManager::GetPropInt("Game.MinVerticalPos");
	int maxVerticalPos = ResourceManager::GetPropInt("Game.MaxVerticalPos");
	int passThreshold = ResourceManager::GetPropInt("Game.PassThreshold");
	mLevel = new GameLevel(mScreenWidth, mScreenHeight, mScreenScaling, columnVerticalSeparation, minVerticalPos, maxVerticalPos, passThreshold, mShiftSpeed, mRenderer, flappyBirdSpriteAtlasName, this, mFlappyBird);

	int bgroundSpriteOrigin = ResourceManager::GetPropFloat("Game.BGroundSpriteOrigin");
	int bgroundSpriteWidth = ResourceManager::GetPropFloat("Game.BGroundSpriteWidth");
	int bgroundSpriteHeight = ResourceManager::GetPropFloat("Game.BGroundSpriteHeight");
	mBackground = new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), bgroundSpriteOrigin, bgroundSpriteOrigin, bgroundSpriteWidth, bgroundSpriteHeight);
	
	int fgroundSpriteOrigin = ResourceManager::GetPropFloat("Game.FGroundSpriteOrigin");
	int fgroundSpriteWidth = ResourceManager::GetPropFloat("Game.FGroundSpriteWidth");
	int fgroundSpriteHeight = ResourceManager::GetPropFloat("Game.FGroundSpriteHeight");
	mForeground = new Sprite(ResourceManager::GetTexture(foregroundTexName), fgroundSpriteOrigin, fgroundSpriteOrigin, fgroundSpriteWidth, fgroundSpriteHeight);
	
	int getReadyX = ResourceManager::GetPropInt("Game.GetReadyX");
	int getReadyY = ResourceManager::GetPropInt("Game.GetReadyY");
	int getReadyWidth = ResourceManager::GetPropInt("Game.GetReadyWidth");
	int getReadyHeight = ResourceManager::GetPropInt("Game.GetReadyHeight");
	mGetReady = new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), getReadyX, getReadyY, getReadyWidth, getReadyHeight);
	
	int instructionsX = ResourceManager::GetPropInt("Game.InstructionsX");
	int instructionsY = ResourceManager::GetPropInt("Game.InstructionsY");
	int instructionsWidth = ResourceManager::GetPropInt("Game.InstructionsWidth");
	int instructionsHeight = ResourceManager::GetPropInt("Game.InstructionsHeight");
	mInstructions = new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), instructionsX, instructionsY, instructionsWidth, instructionsHeight);
	
	int titleX = ResourceManager::GetPropInt("Game.TitleX");
	int titleY = ResourceManager::GetPropInt("Game.TitleY");
	int titleWidth = ResourceManager::GetPropInt("Game.TitleWidth");
	int titleHeight = ResourceManager::GetPropInt("Game.TitleHeight");
	mTitle = new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), titleX, titleY, titleWidth, titleHeight);
	
	int playButtonX = ResourceManager::GetPropInt("Game.PlayButtonX");
	int playButtonY = ResourceManager::GetPropInt("Game.PlayButtonY");
	int playButtonWidth = ResourceManager::GetPropInt("Game.PlayButtonWidth");
	int playButtonHeight = ResourceManager::GetPropInt("Game.PlayButtonHeight");
	mPlayButton = new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), playButtonX, playButtonY, playButtonWidth, playButtonHeight);
	
	int gameOverX = ResourceManager::GetPropFloat("Game.GameOverX");
	int gameOverY = ResourceManager::GetPropFloat("Game.GameOverY");
	int gameOverWidth = ResourceManager::GetPropFloat("Game.GameOverWidth");
	int gameOverHeight = ResourceManager::GetPropFloat("Game.GameOverHeight");
	mGameOver = new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), gameOverX, gameOverY, gameOverWidth, gameOverHeight);

	mGUIScore = new GUIScore(mScreenWidth, mScreenHeight, mScreenScaling);
}

void Game::ProcessInput()
{
	if (mKeys[GLFW_KEY_SPACE] && !mKeysProcessed[GLFW_KEY_SPACE])
	{
		mFlappyBird->mJumpPressed = true;
		mKeysProcessed[GLFW_KEY_SPACE] = true;
	}
}

void Game::Update(float dt)
{
	mFlappyBird->UpdatePosition(dt);
	mLevel->UpdateColumnsPosition(dt);

	DoCollissions();
}

void Game::Render(float dt)
{
	mRenderer->DrawSprite(mBackground, glm::vec2(0.0f), 0.0f);
	mLevel->DrawLevel(dt);
	mFlappyBird->Draw(mRenderer, dt);
	mRenderer->DrawSpriteShifted(mForeground, glm::vec2(0.0f), 0.0f, -mShiftSpeed * dt);
	mGUIScore->DrawBigScoreNumbers(mScore, mRenderer);

	mRenderer->DrawSprite(mGetReady, glm::vec2((mScreenWidth - mGetReady->GetWidth() * mScreenScaling) * mGetReadyScreenFactorX , mScreenHeight * mGetReadyScreenFactorY), 0.0f);
	mRenderer->DrawSprite(mInstructions, glm::vec2((mScreenWidth - mInstructions->GetWidth() * mScreenScaling) * mInstrucScreenFactorX, mScreenHeight * mInstrucScreenFactorY), 0.0f);
	mRenderer->DrawSprite(mTitle, glm::vec2((mScreenWidth - mTitle->GetWidth() * mScreenScaling) * mTitleScreenFactorX, mScreenHeight * mTitleScreenFactorY), 0.0f);
	mRenderer->DrawSprite(mPlayButton, glm::vec2((mScreenWidth - mPlayButton->GetWidth() * mScreenScaling) * mPlayBtnScreenFactorX, mScreenHeight * mPlayBtnScreenFactorY), 0.0f);
	mRenderer->DrawSprite(mGameOver, glm::vec2((mScreenWidth - mGameOver->GetWidth() * mScreenScaling) * mGameOverScreenFactorX, mScreenHeight * mGameOverScreenFactorY), 0.0f);
	mGUIScore->DrawScoreBoard(mScore, mRenderer);
}

void Game::ComputeScore()
{
	mScore++;
}

void Game::LoadProperties()
{
	mShiftSpeed = ResourceManager::GetPropFloat("Game.ShiftSpeed");
	mGetReadyScreenFactorX = ResourceManager::GetPropFloat("Game.GetReadyScreenFactorX");
	mGetReadyScreenFactorY = ResourceManager::GetPropFloat("Game.GetReadyScreenFactorY");
	mInstrucScreenFactorX = ResourceManager::GetPropFloat("Game.InstrucScreenFactorX");
	mInstrucScreenFactorY = ResourceManager::GetPropFloat("Game.InstrucScreenFactorY");
	mPlayBtnScreenFactorX = ResourceManager::GetPropFloat("Game.PlayBtnScreenFactorX");
	mPlayBtnScreenFactorY = ResourceManager::GetPropFloat("Game.PlayBtnScreenFactorY");
	mGameOverScreenFactorX = ResourceManager::GetPropFloat("Game.GameOverScreenFactorX");
	mGameOverScreenFactorY = ResourceManager::GetPropFloat("Game.GameOverScreenFactorY");
}

void Game::DoCollissions()
{
	CheckCollissions(mFlappyBird, mForeground->GetHeight());

	for (int i = 0; i < mLevel->mNumColumnPairs; i++)
	{
		if (mLevel->mColumns[i].first->mPosition.x > mScreenWidth * mScreenScaling)
		{
			break;
		}
		CheckCollissions(mFlappyBird, mLevel->mColumns[i].first);
		CheckCollissions(mFlappyBird, mLevel->mColumns[i].second);
	}
}

void Game::CheckCollissions(BirdGameObject *bird, ColumnGameObject *column)
{
	Sprite *birdSprite = bird->mAnimation->mSprites[0];
	glm::vec2 birdCentre = bird->mPosition + glm::vec2(birdSprite->GetWidth() * 0.5f * mScreenScaling, birdSprite->GetHeight() * 0.5f * mScreenScaling);

	Sprite *colSprite = column->mAnimation->mSprites[0];
	glm::vec2 colCentre = column->mPosition + glm::vec2(colSprite->GetWidth() * 0.5f * mScreenScaling, colSprite->GetHeight() * 0.5f * mScreenScaling);

	glm::vec2 colToBirdVector = birdCentre - colCentre;

	glm::vec2 minHalfExtents(-colSprite->GetWidth() * 0.5f * mScreenScaling, -colSprite->GetHeight() * 0.5f * mScreenScaling);
	glm::vec2 maxHalfExtents(colSprite->GetWidth() * 0.5f * mScreenScaling, colSprite->GetHeight() * 0.5f * mScreenScaling);
	glm::vec2 closestPoint = colCentre + glm::clamp(colToBirdVector, minHalfExtents, maxHalfExtents);
	
	if (glm::length(birdCentre - closestPoint) < birdSprite->GetHeight() * mScreenScaling)
	{
		//std::cout << "Collision with column" << std::endl;
	}
}

void Game::CheckCollissions(BirdGameObject *bird, int groundHeight)
{
	if (bird->mPosition.y < groundHeight * mScreenScaling)
	{
		//std::cout << "Collission with ground" << std::endl;
	}
}

void Game::Clear()
{
	DELETE_PTR(mRenderer);
	DELETE_PTR(mFlappyBird);
	DELETE_PTR(mBackground);
	DELETE_PTR(mForeground);
	DELETE_PTR(mGetReady);
	DELETE_PTR(mInstructions);
	DELETE_PTR(mTitle);
	DELETE_PTR(mPlayButton);
	DELETE_PTR(mScoreBoard);
	DELETE_PTR(mGameOver);
	DELETE_PTR(mGUIScore);
}