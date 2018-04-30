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

	mSoundEngine = irrklang::createIrrKlangDevice();

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

	std::vector<Sprite*> foregroundSprite;
	int fgroundSpriteOrigin = ResourceManager::GetPropInt("Game.FGroundSpriteOrigin");
	int fgroundSpriteWidth = ResourceManager::GetPropInt("Game.FGroundSpriteWidth");
	int fgroundSpriteHeight = ResourceManager::GetPropInt("Game.FGroundSpriteHeight");
	foregroundSprite.push_back(new Sprite(ResourceManager::GetTexture(foregroundTexName), fgroundSpriteOrigin, fgroundSpriteOrigin, fgroundSpriteWidth, fgroundSpriteHeight));
	Animation *foregroundAnim = new Animation(foregroundSprite, 0.0f);
	mForeground = new GameObject(glm::vec2(0.0f), 0.0f, glm::vec2(0.0f), foregroundAnim);

	int bgroundSpriteOrigin = ResourceManager::GetPropInt("Game.BGroundSpriteOrigin");
	int bgroundSpriteWidth = ResourceManager::GetPropInt("Game.BGroundSpriteWidth");
	int bgroundSpriteHeight = ResourceManager::GetPropInt("Game.BGroundSpriteHeight");
	mBackground = new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), bgroundSpriteOrigin, bgroundSpriteOrigin, bgroundSpriteWidth, bgroundSpriteHeight);
	
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
	
	int gameOverX = ResourceManager::GetPropInt("Game.GameOverX");
	int gameOverY = ResourceManager::GetPropInt("Game.GameOverY");
	int gameOverWidth = ResourceManager::GetPropInt("Game.GameOverWidth");
	int gameOverHeight = ResourceManager::GetPropInt("Game.GameOverHeight");
	mGameOver = new Sprite(ResourceManager::GetTexture(flappyBirdSpriteAtlasName), gameOverX, gameOverY, gameOverWidth, gameOverHeight);

	int pressSpaceX = ResourceManager::GetPropInt("Game.PressSpaceX");
	int pressSpaceY = ResourceManager::GetPropInt("Game.PressSpaceY");
	int pressSpaceWidth = ResourceManager::GetPropInt("Game.PressSpaceWidth");
	int pressSpaceHeight = ResourceManager::GetPropInt("Game.PressSpaceHeight");
	std::string pressSpaceTexName = ResourceManager::GetPropString("Game.PressSpaceTexName");
	std::string pressSpaceTexPath = ResourceManager::GetPropString("Game.PressSpaceTexPath");
	ResourceManager::LoadTexture(pressSpaceTexName, pressSpaceTexPath);
	mPressSpace = new Sprite(ResourceManager::GetTexture(pressSpaceTexName), pressSpaceX, pressSpaceY, pressSpaceWidth, pressSpaceHeight);

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
			mFlappyBird->mPosition.x = mScreenWidth * mFactorStartPosX;
			mFlappyBird->mPosition.y = mScreenHeight * mFactorStartPosY;
			mGameState = GameState::INSTRUCTIONS;
		}
		break;
	case GameState::INSTRUCTIONS:
		if (mKeys[GLFW_KEY_SPACE] && !mKeysProcessed[GLFW_KEY_SPACE])
		{
			mKeysProcessed[GLFW_KEY_SPACE] = true;
			mFlappyBird->mJumpPressed = true;
			mSoundEngine->play2D(mFlyAudioPath.c_str());
			mGameState = GameState::ACTIVE;
		}
		break;
	case GameState::ACTIVE:
		if (mKeys[GLFW_KEY_SPACE] && !mKeysProcessed[GLFW_KEY_SPACE])
		{
			mKeysProcessed[GLFW_KEY_SPACE] = true;
			mFlappyBird->mJumpPressed = true;
			mSoundEngine->play2D(mFlyAudioPath.c_str());
		}
		break;
	case GameState::SHOW_SCORE:
		if (mIsSBShowed && mKeys[GLFW_KEY_SPACE] && !mKeysProcessed[GLFW_KEY_SPACE])
		{
			mIsSBShowed = false;
			mKeysProcessed[GLFW_KEY_SPACE] = true;
			mScore = 0;
			mFlappyBird->mPosition.x = mScreenWidth * mFactorStartPosX;
			mFlappyBird->mPosition.y = mScreenHeight * mFactorStartPosY;
			mFlappyBird->mRotInDegrees = 0.0f;
			mLevel->Reset();
			mRenderer->ResetAlpha();
			mGUIScore->ResetSBHeightPos();
			mGUIMovAudioPlayed = false;
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

	case GameState::DEAD:
		mDeadTimer += dt;
		mLevel->BeginShake();
		mFlappyBird->BeginShake();
		mForeground->BeginShake();

		if (mDeadTimer > mFactorShakeTime * mMaxDeadTimer)
		{
			if (!mDieAudioPlayed)
			{
				mSoundEngine->play2D(mDieAudioPath.c_str());
				mDieAudioPlayed = true;
			}
			mLevel->EndShake();
			mFlappyBird->EndShake();
			mForeground->EndShake();
		}

		if (mDeadTimer > mMaxDeadTimer)
		{
			mDeadTimer = 0.0f;
			mHasDeadTimerExpired = true;
			mDieAudioPlayed = false;
				 
			if (mScore > mGUIScore->GetBestScore())
			{
				mGUIScore->SaveMaxScore(mScore);
			}

			mGameState = GameState::SHOW_SCORE;
			break;
		}
	}
}

void Game::Render(float dt)
{
	mRenderer->DrawSprite(mBackground, glm::vec2(0.0f), 0.0f);

	switch (mGameState)
	{
		case GameState::INITIAL_SCREEN:
		{
			mFlappyBird->Draw(mRenderer, dt);

			float titlePosX = (mScreenWidth - mTitle->GetWidth() * mScreenScaling) * mFactorTitleScreenX;
			float titlePosY = mScreenHeight * mFactorTitleScreenY;
			mRenderer->DrawSprite(mTitle, glm::vec2(titlePosX, titlePosY), 0.0f);

			float playBtnPosX = (mScreenWidth - mPlayButton->GetWidth() * mScreenScaling) * mFactorPlayBtnScreenX;
			float playBtnPosY = mScreenHeight * mFactorPlayBtnScreenY;
			mRenderer->DrawSprite(mPlayButton, glm::vec2(playBtnPosX, playBtnPosY), 0.0f);
			mRenderer->DrawSpriteShifted(mForeground->mAnimation->mSprites[0], glm::vec2(0.0f), -mShiftSpeed * dt);

			float pressSpaceX = (mScreenWidth - mPressSpace->GetWidth() * mScreenScaling) * mFactorPressSpaceX;
			float pressSpaceY = mScreenHeight * mFactorPressSpaceY;
			mRenderer->DrawSprite(mPressSpace, glm::vec2(pressSpaceX, pressSpaceY), 0.0f);

			break;
		}
		case GameState::INSTRUCTIONS:
		{
			mFlappyBird->Draw(mRenderer, dt);

			mGUIScore->DrawBigScoreNumbers(mScore, mRenderer);

			float getReadyPosX = (mScreenWidth - mGetReady->GetWidth() * mScreenScaling) * mFactorGetReadyScreenX;
			float getReadyPosY = mScreenHeight * mFactorGetReadyScreenY;
			mRenderer->DrawSprite(mGetReady, glm::vec2( getReadyPosX, getReadyPosY), 0.0f);

			float instrucPosX = (mScreenWidth - mInstructions->GetWidth() * mScreenScaling) * mFactorInstrucScreenX;
			float instrucPosY = mScreenHeight * mFactorInstrucScreenY;
			mRenderer->DrawSpriteFade(mInstructions, glm::vec2(instrucPosX, instrucPosY), true, dt);
			mRenderer->DrawSpriteShifted(mForeground->mAnimation->mSprites[0], glm::vec2(0.0f), -mShiftSpeed * dt);

			float pressSpaceX = (mScreenWidth - mPressSpace->GetWidth() * mScreenScaling) * mFactorPressSpaceX;
			float pressSpaceY = mScreenHeight * mFactorPressSpaceY;
			mRenderer->DrawSprite(mPressSpace, glm::vec2(pressSpaceX, pressSpaceY), 0.0f);

			break;
		}
		case GameState::ACTIVE:
		{
			mLevel->DrawLevel(dt);
			mFlappyBird->Draw(mRenderer, dt);
			mGUIScore->DrawBigScoreNumbers(mScore, mRenderer);
			float instrucPosX = (mScreenWidth - mInstructions->GetWidth() * mScreenScaling) * mFactorInstrucScreenX;
			float instrucPosY = mScreenHeight * mFactorInstrucScreenY;
			mRenderer->DrawSpriteFade(mInstructions, glm::vec2(instrucPosX, instrucPosY), false, dt);
			mRenderer->DrawSpriteShifted(mForeground->mAnimation->mSprites[0], glm::vec2(0.0f), -mShiftSpeed * dt);

			break;
		}
		case GameState::DEAD:
		{
			mLevel->DrawLevel(0.0f);
			mFlappyBird->Draw(mRenderer, 0.0f);
			mRenderer->DrawSpriteShifted(mForeground->mAnimation->mSprites[0], mForeground->mPosition, 0.0f);
			break;
		}
		case GameState::SHOW_SCORE:
		{
			mLevel->DrawLevel(0.0f);

			float gameOverPosX = (mScreenWidth - mGameOver->GetWidth() * mScreenScaling) * mFactorGameOverScreenX;
			float gameOverPosY = mScreenHeight * mFactorGameOverScreenY;
			bool fadeFinished = mRenderer->DrawSpriteFade(mGameOver, glm::vec2(gameOverPosX, gameOverPosY), true, dt);
			
			mRenderer->DrawSpriteShifted(mForeground->mAnimation->mSprites[0], glm::vec2(0.0f), 0.0f);
			
			if (fadeFinished)
			{
				if (!mGUIMovAudioPlayed)
				{
					mSoundEngine->play2D(mGUIMovementAudioPath.c_str());
					mGUIMovAudioPlayed = true;
				}			
				bool drawFinished = mGUIScore->DrawScoreBoard(mScore, mRenderer, dt);
				if (drawFinished)
				{
					float playBtnPosX = (mScreenWidth - mPlayButton->GetWidth() * mScreenScaling) * mFactorPlayBtnScreenX;
					float playBtnPosY = mScreenHeight * mFactorPlayBtnScreenY;
					mRenderer->DrawSprite(mPlayButton, glm::vec2(playBtnPosX, playBtnPosY), 0.0f);
					mIsSBShowed = true;
				}
			}

			float pressSpaceX = (mScreenWidth - mPressSpace->GetWidth() * mScreenScaling) * mFactorPressSpaceX;
			float pressSpaceY = mScreenHeight * mFactorPressSpaceY;
			mRenderer->DrawSprite(mPressSpace, glm::vec2(pressSpaceX, pressSpaceY), 0.0f);

			break;
		}
		
	}
}

void Game::ComputeScore()
{
	mSoundEngine->play2D(mScorePointAudioPath.c_str());
	mScore++;
}

void Game::LoadProperties()
{
	mFactorPressSpaceX = ResourceManager::GetPropFloat("Game.FactorPressSpaceX");
	mFactorPressSpaceY = ResourceManager::GetPropFloat("Game.FactorPressSpaceY");
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
	mMaxDeadTimer = ResourceManager::GetPropFloat("Game.MaxDeadTimer");
	mFactorShakeTime = ResourceManager::GetPropFloat("Game.FactorShakeTime");
	mFlyAudioPath = ResourceManager::GetPropString("Game.FlyAudioPath");
	mCollissionAudioPath = ResourceManager::GetPropString("Game.CollissionAudioPath");
	mGUIMovementAudioPath = ResourceManager::GetPropString("Game.GUIMovementAudioPath");
	mScorePointAudioPath = ResourceManager::GetPropString("Game.ScorePointAudioPath");
	mDieAudioPath = ResourceManager::GetPropString("Game.DieAudioPath");
}

void Game::DoCollissions()
{
	CheckCollissions(mFlappyBird, mForeground->mAnimation->mSprites[0]->GetHeight());

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
	
	if (glm::length(birdCentre - closestPoint) < birdSprite->GetHeight() * 0.5f * mScreenScaling)
	{
		mSoundEngine->play2D(mCollissionAudioPath.c_str());
		mLevel->PrepareShake();
		mFlappyBird->PrepareShake();
		mForeground->PrepareShake();
		mGameState = GameState::DEAD;

	}
}

void Game::CheckCollissions(BirdGameObject *bird, int groundHeight)
{
	if (bird->mPosition.y < groundHeight * mScreenScaling)
	{
		mSoundEngine->play2D(mCollissionAudioPath.c_str());
		mLevel->PrepareShake();
		mFlappyBird->PrepareShake();
		mForeground->PrepareShake();
		mGameState = GameState::DEAD;
	}
}

void Game::Clear()
{
	DELETE_PTR(mRenderer);
	DELETE_PTR(mFlappyBird);
	DELETE_PTR(mLevel);
	DELETE_PTR(mBackground);
	DELETE_PTR(mForeground);
	DELETE_PTR(mGetReady);
	DELETE_PTR(mInstructions);
	DELETE_PTR(mTitle);
	DELETE_PTR(mPlayButton);
	DELETE_PTR(mScoreBoard);
	DELETE_PTR(mGameOver);
	DELETE_PTR(mPressSpace);
	DELETE_PTR(mGUIScore);
	
	mSoundEngine->drop();

}