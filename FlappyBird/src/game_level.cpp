#include "game_level.h"
#include <glm/glm.hpp>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include "sprite.h"
#include "animation.h"
#include "column_game_object.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "globals.h"

GameLevel::GameLevel(int screenWidth, int screenHeight, float screenScaling, int verticalSeparation, 
	int minVerticalPos, int maxVerticalPos, int passThreshold, int shiftSpeedX, 
	const SpriteRenderer *spriteRenderer, const std::string &flappyBirdSpriteAtlasName, Game *game, const BirdGameObject *bird) :
	mScreenWidth(screenWidth),
	mScreenHeight(screenHeight),
	mScreenScaling(screenScaling),
	mVerticalSeparation(verticalSeparation),
	mMinVerticalPos(minVerticalPos), 
	mMaxVerticalPos(maxVerticalPos),
	mPassThreshold(passThreshold),
	mShiftSpeedX(shiftSpeedX),
	mSpriteRenderer(spriteRenderer),
	mFlappyBirdSpriteAtlasName(flappyBirdSpriteAtlasName),
	mGame(game),
	mBird(bird)
{	
	Init();
}

GameLevel::~GameLevel()
{
	Clear();
}

void GameLevel::Init()
{
	mNumColumnPairs = ResourceManager::GetPropInt("GameLevel.NumColumnPairs");
	mFactorHorizSeparation = ResourceManager::GetPropInt("GameLevel.FactorHorizSeparation");

	mSpriteWidth = ResourceManager::GetPropInt("GameLevel.SpriteWidth");// 26;
	mSpriteHeight = ResourceManager::GetPropInt("GameLevel.SpriteHeight");//160;

	int lowerColX = ResourceManager::GetPropInt("GameLevel.LowerColX"); //84
	int lowerColY = ResourceManager::GetPropInt("GameLevel.LowerColY"); //323
	
	int upperColX = ResourceManager::GetPropInt("GameLevel.UpperColX"); //56
	int upperColY = ResourceManager::GetPropInt("GameLevel.UpperColY"); //323
		
	float posX;
	float posYLower;
	float separationDistanceY;

	glm::vec2 posLowerCol;
	glm::vec2 posUpperCol;

	srand(time(NULL));

	for (int i = 0; i < mNumColumnPairs; i++)
	{
		if (i == 0)
		{
			posX = (float)mScreenWidth;
			posYLower = 0.0f; 
			separationDistanceY = mVerticalSeparation * mScreenScaling;
		}
		else
		{
			ColumnPair &pair = mColumns.back();

			posYLower = mMinVerticalPos + (mMaxVerticalPos - mMinVerticalPos) * (float)rand() / (float)RAND_MAX;
			posYLower *= mScreenScaling;
			
			posX = ComputeXPos(pair.first->mPosition.x, pair.first->mPosition.y, pair.second->mPosition.y, posYLower, posYLower + mSpriteHeight * mScreenScaling + separationDistanceY);
		}

		posLowerCol.x = posX;
		posLowerCol.y = posYLower;
		posUpperCol.x = posX;
		posUpperCol.y = posYLower + mSpriteHeight * mScreenScaling + separationDistanceY;

		Sprite *lowerColumnSprite = new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), lowerColX, lowerColY, mSpriteWidth, mSpriteHeight);
		std::vector<Sprite*> lowerColumnSpriteVector = { lowerColumnSprite };

		Sprite *upperColumnSprite = new Sprite(ResourceManager::GetTexture(mFlappyBirdSpriteAtlasName), upperColX, upperColY, mSpriteWidth, mSpriteHeight);
		std::vector<Sprite*> upperColumnSpriteVector = { upperColumnSprite };

		Animation *lowerColumnAnim = new Animation(lowerColumnSpriteVector, 0.0f);
		Animation *upperColumnAnim = new Animation(upperColumnSpriteVector, 0.0f);

		ColumnGameObject *lowerColumnGO = new ColumnGameObject(posLowerCol, 0.0f, glm::vec2(mShiftSpeedX, 0.0f), lowerColumnAnim, mBird, mGame, mScreenScaling, true);
		ColumnGameObject *upperColumnGO = new ColumnGameObject(posUpperCol, 0.0f, glm::vec2(mShiftSpeedX, 0.0f), upperColumnAnim, mBird, mGame, mScreenScaling, false);

		ColumnPair columnPair = std::make_pair(lowerColumnGO, upperColumnGO);
		mColumns.push_back(columnPair);
	}
}

void GameLevel::UpdateColumnsPosition(float dt)
{
	for (ColumnPair &pair : mColumns)
	{
		pair.first->UpdatePosition(dt);
		pair.second->UpdatePosition(dt);
	}

	ColumnPair &firstPair = mColumns.front();

	//Sprite is out of the screen boundaries
	if (firstPair.first->mPosition.x + mSpriteWidth * mScreenScaling < 0.0f)
	{
		ResetColumnPairPosition(); 
	}
}

void GameLevel::ResetColumnPairPosition()
{
	ColumnPair columnOut = mColumns.front();
	mColumns.pop_front();

	float posYLower = mMinVerticalPos + (mMaxVerticalPos - mMinVerticalPos) * (float)rand() / (float)RAND_MAX;
	posYLower *= mScreenScaling;

	float separationDistanceY = mVerticalSeparation * mScreenScaling;

	ColumnPair &lastPair = mColumns.back();

	float posX = ComputeXPos(lastPair.first->mPosition.x, lastPair.first->mPosition.y, lastPair.second->mPosition.y, posYLower, posYLower + mSpriteHeight * mScreenScaling + separationDistanceY);
	
	glm::vec2 posLowerCol(posX, posYLower);
	glm::vec2 posUpperCol(posX, posYLower + mSpriteHeight * mScreenScaling + separationDistanceY);

	columnOut.first->mPosition = posLowerCol;
	columnOut.second->mPosition = posUpperCol;

	columnOut.first->ResetScoreComputed();
	
	mColumns.push_back(columnOut);
}

void GameLevel::DrawLevel(float dt) const
{
	for (const ColumnPair &pair : mColumns)
	{
		
		if (pair.first->mPosition.x > mScreenWidth)
		{
			break;
		}
		
		pair.first->Draw(mSpriteRenderer, dt);
		pair.second->Draw(mSpriteRenderer, dt);
	}
}

float GameLevel::GetSpriteWidth() const
{
	return mSpriteWidth;
}

void GameLevel::Reset()
{
	float posX;
	float posYLower;
	float separationDistanceY = mVerticalSeparation * mScreenScaling;

	glm::vec2 posLowerCol;
	glm::vec2 posUpperCol;

	for (int i = 0; i < mNumColumnPairs; i++)
	{
		if (i == 0)
		{
			//Reset first column pair
			posX = (float)mScreenWidth;
			posYLower = 0.0f;
		}
		else
		{
			ColumnPair &previousPair = mColumns[i-1];

			posYLower = mMinVerticalPos + (mMaxVerticalPos - mMinVerticalPos) * (float)rand() / (float)RAND_MAX;
			posYLower *= mScreenScaling;

			posX = ComputeXPos(previousPair.first->mPosition.x, previousPair.first->mPosition.y, previousPair.second->mPosition.y, posYLower, posYLower + mSpriteHeight * mScreenScaling + separationDistanceY);
		}

		//Lower column
		mColumns[i].first->mPosition.x = posX;
		mColumns[i].first->mPosition.y = posYLower;
		//Upper column
		mColumns[i].second->mPosition.x = posX;
		mColumns[i].second->mPosition.y = posYLower + mSpriteHeight * mScreenScaling + separationDistanceY;
	}
}

void GameLevel::PrepareShake()
{
	for (int i = 0; i < mColumns.size(); i++)
	{
		mColumns[i].first->PrepareShake();
		mColumns[i].second->PrepareShake();
	}
}

void GameLevel::BeginShake()
{
	for (int i = 0; i < mColumns.size(); i++)
	{
		mColumns[i].first->BeginShake();
		mColumns[i].second->BeginShake();
	}
}

void GameLevel::EndShake()
{
	for (int i = 0; i < mColumns.size(); i++)
	{
		mColumns[i].first->EndShake();
		mColumns[i].second->EndShake();
	}
}

void GameLevel::Clear()
{
	for (int i = 0; i < mNumColumnPairs; i++)
	{
		ColumnPair &pair = mColumns[i];
		DELETE_PTR(pair.first);
		DELETE_PTR(pair.second);	
	}
	mColumns.clear();
}

float GameLevel::ComputeXPos(float posLowerLeftX, float posLowerLeftY, float posUpperLeftY, float posLowerRightY, float posUpperRightY) const
{
	float posLowerLeft =  posLowerLeftY + mSpriteHeight * mScreenScaling;
	float posLowerRight = posLowerRightY + mSpriteHeight * mScreenScaling;

	if (posLowerRight >= posLowerLeft)
	{
		if (posUpperLeftY - posLowerRight < mPassThreshold * mScreenScaling)
		{
			return posLowerLeftX + mFactorHorizSeparation * mSpriteWidth * mScreenScaling;
		}
		
		return posLowerLeftX + (rand() % mFactorHorizSeparation + 1) * mSpriteWidth * mScreenScaling;
	}
	else
	{
		if (posUpperRightY - posLowerLeft < mPassThreshold * mScreenScaling)
		{
			return posLowerLeftX + mFactorHorizSeparation * mSpriteWidth * mScreenScaling;
		}
		
		return posLowerLeftX + (rand() % mFactorHorizSeparation + 1) * mSpriteWidth * mScreenScaling;
	}
}


