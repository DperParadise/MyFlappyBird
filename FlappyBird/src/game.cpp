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

//TEST
#include <iostream>

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

	ResourceManager::LoadTexture(mFlappyBirdSpriteAtlasName, mFlappyBirdSpriteAtlasPath);
	ResourceManager::LoadTexture(mForegroundTexName, mForegroundTexPath);
	ResourceManager::LoadShader(mGameShaderName, mVertexShaderPath, mFragmentShaderPath);
	glm::mat4 projection;
	projection = glm::ortho(0.0f, float(mScreenWidth), 0.0f, float(mScreenHeight), -1.0f, 1.0f);
	ResourceManager::GetShader(mGameShaderName).SetMatrix4("projection", projection);
	ResourceManager::GetShader(mGameShaderName).SetInteger("textureSampler", 0);

	mRenderer = new SpriteRenderer(ResourceManager::GetShader(mGameShaderName), mScreenScaling);
																							
	std::vector<Sprite*> birdSprites;
	birdSprites.push_back(new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), mBirdSprite1X, mBirdSpriteY, mBirdSpriteWidth, mBirdSpriteHeight));
	birdSprites.push_back(new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), mBirdSprite2X, mBirdSpriteY, mBirdSpriteWidth, mBirdSpriteHeight));
	birdSprites.push_back(new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), mBirdSprite3X, mBirdSpriteY, mBirdSpriteWidth, mBirdSpriteHeight));
	Animation *flyAnimation = new Animation(birdSprites, mBirdAnimSpeed);
	mFlappyBird = new BirdGameObject(mStartPos, mStartRot, glm::vec2(0.0f, 0.0f), mWorldAcceleration, flyAnimation);
		
	mLevel = new GameLevel(mScreenWidth, mScreenHeight, mScreenScaling, mColumnVerticalSeparation, mMinVerticalPos, mMaxVerticalPos, mPassThreshold, mShiftSpeed, mRenderer, mFlappyBirdSpriteAtlasName, this, mFlappyBird);

	mBackground = new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), mBGroundSpriteOrigin, mBGroundSpriteOrigin, mBGroundSpriteWidth, mBGroundSpriteHeight);
	mForeground = new Sprite(ResourceManager::GetTexture(mForegroundTexName), mFGroundSpriteOrigin, mFGroundSpriteOrigin, mFGroundSpriteWidth, mFGroundSpriteHeight);
	mGetReady = new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), mGetReadyX, mGetReadyY, mGetReadyWidth, mGetReadyHeight);
	mInstructions = new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), mInstructionsX, mInstructionsY, mInstructionsWidth, mInstructionsHeight);
	mTitle = new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), mTitleX, mTitleY, mTitleWidth, mTitleHeight);
	mPlayButton = new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), mPlayButtonX, mPlayButtonY, mPlayButtonWidth, mPlayButtonHeight);
	mScoreBoard = new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), mScoreBoardX, mScoreBoardY, mScoreBoardWidth, mScoreBoardHeight);

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
	mGUIScore->Draw(mScore, mRenderer);

	mRenderer->DrawSprite(mGetReady, glm::vec2((mScreenWidth - mGetReady->GetWidth() * mScreenScaling) * mGetReadyScreenFactorX , mScreenHeight * mGetReadyScreenFactorY), 0.0f);
	//mRenderer->DrawSprite(mInstructions, glm::vec2((mScreenWidth - mInstructions->GetWidth() * mScreenScaling) * mInstrucScreenFactorX, mScreenHeight * mInstrucScreenFactorY), 0.0f);
	mRenderer->DrawSprite(mTitle, glm::vec2((mScreenWidth - mTitle->GetWidth() * mScreenScaling) * mTitleScreenFactorX, mScreenHeight * mTitleScreenFactorY), 0.0f);
	mRenderer->DrawSprite(mPlayButton, glm::vec2((mScreenWidth - mPlayButton->GetWidth() * mScreenScaling) * mPlayBtnScreenFactorX, mScreenHeight * mPlayBtnScreenFactorY), 0.0f);
	mRenderer->DrawSprite(mScoreBoard, glm::vec2((mScreenWidth - mScoreBoard->GetWidth() * mScreenScaling) * mScoreBrdScreenFactorX, mScreenHeight * mScoreBrdScreenFactorY), 0.0f);
}

void Game::ComputeScore()
{
	mScore++;
	std::cout << "score = " << mScore << std::endl;
}

void Game::LoadProperties()
{
	mFlappyBirdSpriteAtlasName = ResourceManager::GetPropString("Game.mFlappyBirdSpriteAtlasName");
	mFlappyBirdSpriteAtlasPath = ResourceManager::GetPropString("Game.mFlappyBirdSpriteAtlasPath");
	mForegroundTexName = ResourceManager::GetPropString("Game.mForegroundTexName");
	mForegroundTexPath = ResourceManager::GetPropString("Game.mForegroundTexPath");
	mGameShaderName = ResourceManager::GetPropString("Game.mGameShaderName");
	mVertexShaderPath = ResourceManager::GetPropString("Game.mVertexShaderPath");
	mFragmentShaderPath = ResourceManager::GetPropString("Game.mFragmentShaderPath");
	mBirdSpriteWidth = ResourceManager::GetPropInt("Game.mBirdSpriteWidth");
	mBirdSpriteHeight = ResourceManager::GetPropInt("Game.mBirdSpriteHeight");
	mBirdSpriteY = ResourceManager::GetPropInt("Game.mBirdSpriteY");
	mBirdSprite1X = ResourceManager::GetPropInt("Game.mBirdSprite1X");
	mBirdSprite2X = ResourceManager::GetPropInt("Game.mBirdSprite2X");
	mBirdSprite3X = ResourceManager::GetPropInt("Game.mBirdSprite3X");
	mBirdAnimSpeed = ResourceManager::GetPropFloat("Game.mBirdAnimSpeed");
	mStartPos.x = ResourceManager::GetPropFloat("Game.mStartPosX");
	mStartPos.y = ResourceManager::GetPropFloat("Game.mStartPosY");
	mStartRot = ResourceManager::GetPropFloat("Game.mStartRot");
	mWorldAcceleration = ResourceManager::GetPropFloat("Game.mWorldAcceleration");
	mColumnVerticalSeparation = ResourceManager::GetPropFloat("Game.mColumnVerticalSeparation");
	mMinVerticalPos = ResourceManager::GetPropFloat("Game.mMinVerticalPos");
	mMaxVerticalPos = ResourceManager::GetPropFloat("Game.mMaxVerticalPos");
	mPassThreshold = ResourceManager::GetPropFloat("Game.mPassThreshold");
	mShiftSpeed = ResourceManager::GetPropFloat("Game.mShiftSpeed");
	mBGroundSpriteOrigin = ResourceManager::GetPropFloat("Game.mBGroundSpriteOrigin");
	mBGroundSpriteWidth = ResourceManager::GetPropFloat("Game.mBGroundSpriteWidth");
	mBGroundSpriteHeight = ResourceManager::GetPropFloat("Game.mBGroundSpriteHeight");
	mFGroundSpriteOrigin = ResourceManager::GetPropFloat("Game.mFGroundSpriteOrigin");
	mFGroundSpriteWidth = ResourceManager::GetPropFloat("Game.mFGroundSpriteWidth");
	mFGroundSpriteHeight = ResourceManager::GetPropFloat("Game.mFGroundSpriteHeight");
	mGetReadyX = ResourceManager::GetPropInt("Game.mGetReadyX");
	mGetReadyY = ResourceManager::GetPropInt("Game.mGetReadyY");
	mGetReadyWidth = ResourceManager::GetPropInt("Game.mGetReadyWidth");
	mGetReadyHeight = ResourceManager::GetPropInt("Game.mGetReadyHeight");
	mGetReadyScreenFactorX = ResourceManager::GetPropFloat("Game.mGetReadyScreenFactorX");
	mGetReadyScreenFactorY = ResourceManager::GetPropFloat("Game.mGetReadyScreenFactorY");
	mInstructionsX = ResourceManager::GetPropInt("Game.mInstructionsX");
	mInstructionsY = ResourceManager::GetPropInt("Game.mInstructionsY");
	mInstructionsWidth = ResourceManager::GetPropInt("Game.mInstructionsWidth");
	mInstructionsHeight = ResourceManager::GetPropInt("Game.mInstructionsHeight");
	mInstrucScreenFactorX = ResourceManager::GetPropFloat("Game.mInstrucScreenFactorX");
	mInstrucScreenFactorY = ResourceManager::GetPropFloat("Game.mInstrucScreenFactorY");
	mTitleX = ResourceManager::GetPropInt("Game.mTitleX");
	mTitleY = ResourceManager::GetPropInt("Game.mTitleY");
	mTitleWidth = ResourceManager::GetPropInt("Game.mTitleWidth");
	mTitleHeight = ResourceManager::GetPropInt("Game.mTitleHeight");
	mPlayBtnScreenFactorX = ResourceManager::GetPropFloat("Game.mPlayBtnScreenFactorX");
	mPlayBtnScreenFactorY = ResourceManager::GetPropFloat("Game.mPlayBtnScreenFactorY");
	mScoreBoardX = ResourceManager::GetPropFloat("Game.mScoreBoardX");
	mScoreBoardY = ResourceManager::GetPropFloat("Game.mScoreBoardY");
	mScoreBoardWidth = ResourceManager::GetPropFloat("Game.mScoreBoardWidth");
	mScoreBoardHeight = ResourceManager::GetPropFloat("Game.mScoreBoardHeight");
	mScoreBrdScreenFactorX = ResourceManager::GetPropFloat("Game.mScoreBrdScreenFactorX");
	mScoreBrdScreenFactorY = ResourceManager::GetPropFloat("Game.mScoreBrdScreenFactorY");
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
	DELETE_PTR(mGUIScore);
}