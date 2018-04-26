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
	
	float factorInitScreenPosX = ResourceManager::GetPropFloat("Game.FactorInitScreenPosX");
	float factorInitScreenPosY = ResourceManager::GetPropFloat("Game.FactorInitiScreenPosY");
	float startRot = ResourceManager::GetPropFloat("Game.StartRot");
	int worldGravity = ResourceManager::GetPropInt("Game.WorldGravity");
	float birdAnimSpeed = ResourceManager::GetPropFloat("Game.BirdAnimSpeed");
	Animation *flyAnimation = new Animation(birdSprites, birdAnimSpeed);
	glm::vec2 initPos(mScreenWidth * factorInitScreenPosX - birdSpriteWidth * mScreenScaling * factorInitScreenPosX , mScreenHeight * factorInitScreenPosY);
	mFlappyBird = new BirdGameObject(initPos, startRot, glm::vec2(0.0f, 0.0f), worldGravity, flyAnimation, mScreenScaling);
		
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
	switch (mGameState)
	{
	case GameState::INITIAL_SCREEN:
		if (mKeys[GLFW_KEY_SPACE] && !mKeysProcessed[GLFW_KEY_SPACE])
		{
			mKeysProcessed[GLFW_KEY_SPACE] = true;
			mGameState = GameState::INSTRUCTIONS;
			mFlappyBird->mPosition.x = mScreenWidth * mFactorStartPosX;
			mFlappyBird->mPosition.y = mScreenHeight * mFactorStartPosY;
		}
		break;
	case GameState::INSTRUCTIONS:
		if (mKeys[GLFW_KEY_SPACE] && !mKeysProcessed[GLFW_KEY_SPACE])
		{
			mKeysProcessed[GLFW_KEY_SPACE] = true;
			mGameState = GameState::ACTIVE;
		}
		break;
	case GameState::ACTIVE:
		if (!mFlappyBird->IsAlive())
		{
			mGameState = GameState::DEAD;
			break;
		}
		if (mKeys[GLFW_KEY_SPACE] && !mKeysProcessed[GLFW_KEY_SPACE])
		{
			mKeysProcessed[GLFW_KEY_SPACE] = true;
			mFlappyBird->mJumpPressed = true;
		}
		break;
	case GameState::DEAD:
		if (mKeys[GLFW_KEY_SPACE] && !mKeysProcessed[GLFW_KEY_SPACE])
		{
			mKeysProcessed[GLFW_KEY_SPACE] = true;
			mFlappyBird->mJumpPressed = true;
			mGameState = GameState::INSTRUCTIONS;
		}
		break;
	}
}

void Game::Update(float dt)
{
	switch (mGameState)
	{
	case GameState::INITIAL_SCREEN:
		mFlappyBird->Hover(dt);
		break;

	case GameState::INSTRUCTIONS:
		mFlappyBird->Hover(dt);
		break;

	case GameState::ACTIVE:
		mFlappyBird->UpdatePosition(dt);
		mLevel->UpdateColumnsPosition(dt);
		DoCollissions();
		break;
	}
}

void Game::Render(float dt)
{
	switch (mGameState)
	{
	case GameState::INITIAL_SCREEN:
		mRenderer->DrawSprite(mBackground, glm::vec2(0.0f), 0.0f);
		
		mFlappyBird->Draw(mRenderer, dt);

		float titlePosX = (mScreenWidth - mTitle->GetWidth() * mScreenScaling) * mFactorTitleScreenX;
		float titlePosY = mScreenHeight * mFactorTitleScreenY;
		mRenderer->DrawSprite(mTitle, glm::vec2(titlePosX, titlePosY), 0.0f);

		float playBtnPosX = (mScreenWidth - mPlayButton->GetWidth() * mScreenScaling) * mFactorPlayBtnScreenX;
		float playBtnPosY = mScreenHeight * mFactorPlayBtnScreenY;
		mRenderer->DrawSprite(mPlayButton, glm::vec2(playBtnPosX, playBtnPosY), 0.0f);

		mRenderer->DrawSpriteShifted(mForeground, glm::vec2(0.0f), 0.0f, -mShiftSpeed * dt);

		break;
	}

/*
	mRenderer->DrawSprite(mBackground, glm::vec2(0.0f), 0.0f);
	mLevel->DrawLevel(dt);
	mFlappyBird->Draw(mRenderer, dt);
	mRenderer->DrawSpriteShifted(mForeground, glm::vec2(0.0f), 0.0f, -mShiftSpeed * dt);
	mGUIScore->DrawBigScoreNumbers(mScore, mRenderer);

	mRenderer->DrawSprite(mGetReady, glm::vec2((mScreenWidth - mGetReady->GetWidth() * mScreenScaling) * mFactorGetReadyScreenX , mScreenHeight * mFactorGetReadyScreenY), 0.0f);
	mRenderer->DrawSprite(mInstructions, glm::vec2((mScreenWidth - mInstructions->GetWidth() * mScreenScaling) * mFactorInstrucScreenX, mScreenHeight * mFactorInstrucScreenY), 0.0f);
	mRenderer->DrawSprite(mTitle, glm::vec2((mScreenWidth - mTitle->GetWidth() * mScreenScaling) * mFactorTitleScreenX, mScreenHeight * mFactorTitleScreenY), 0.0f);
	mRenderer->DrawSprite(mPlayButton, glm::vec2((mScreenWidth - mPlayButton->GetWidth() * mScreenScaling) * mFactorPlayBtnScreenX, mScreenHeight * mFactorPlayBtnScreenY), 0.0f);
	mRenderer->DrawSprite(mGameOver, glm::vec2((mScreenWidth - mGameOver->GetWidth() * mScreenScaling) * mFactorGameOverScreenX, mScreenHeight * mFactorGameOverScreenY), 0.0f);
	mGUIScore->DrawScoreBoard(mScore, mRenderer);
	*/
}

void Game::ComputeScore()
{
	mScore++;
}

void Game::LoadProperties()
{
	mFactorStartPosX = ResourceManager::GetPropFloat("Game.FactorStartPosX");
	mFactorStartPosY = ResourceManager::GetPropFloat("Game.FactorStartPosY");
	mShiftSpeed = ResourceManager::GetPropFloat("Game.ShiftSpeed");
	mFactorGetReadyScreenX = ResourceManager::GetPropFloat("Game.FactorGetReadyScreenX");
	mFactorGetReadyScreenY = ResourceManager::GetPropFloat("Game.FactorGetReadyScreenY");
	mFactorInstrucScreenX = ResourceManager::GetPropFloat("Game.FactorInstrucScreenX");
	mFactorInstrucScreenY = ResourceManager::GetPropFloat("Game.FactorInstrucScreenY");
	mFactorPlayBtnScreenX = ResourceManager::GetPropFloat("Game.FactorPlayBtnScreenX");
	mFactorPlayBtnScreenY = ResourceManager::GetPropFloat("Game.FactorPlayBtnScreenY");
	mFactorGameOverScreenX = ResourceManager::GetPropFloat("Game.FactorGameOverScreenX");
	mFactorGameOverScreenY = ResourceManager::GetPropFloat("Game.FactorGameOverScreenY");
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