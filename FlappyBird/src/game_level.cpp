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

GameLevel::GameLevel(int screenWidth, int screenHeight, float screenScaling, float minVerticalSeparation, float maxVerticalSeparation, float minVerticalPos, float maxVerticalPos, float passThreshold, float shiftSpeedX, const SpriteRenderer *spriteRenderer) :
	mScreenWidth(screenWidth),
	mScreenHeight(screenHeight),
	mScreenScaling(screenScaling),
	mMinVerticalSeparation(minVerticalSeparation),
	mMaxVerticalSeparation(maxVerticalSeparation),
	mMinVerticalPos(minVerticalPos), 
	mMaxVerticalPos(maxVerticalPos),
	mPassThreshold(passThreshold),
	mShiftSpeedX(shiftSpeedX),
	mSpriteRenderer(spriteRenderer)
{	
	Init();
}

GameLevel::~GameLevel()
{
	Clear();
}

void GameLevel::Init()
{
	mSpriteWidth = 26;
	mSpriteHeight = 160;

	Sprite *lowerColumnSprite = new Sprite(ResourceManager::GetTexture("flappyBirdSpriteAtlas"), 84, 323, mSpriteWidth, mSpriteHeight);
	Sprite *upperColumnSprite = new Sprite(ResourceManager::GetTexture("flappyBirdSpriteAtlas"), 56, 323, mSpriteWidth, mSpriteHeight);
	
	std::vector<Sprite*> lowerColumnSpriteVector = { lowerColumnSprite };
	std::vector<Sprite*> upperColumnSpriteVector = { upperColumnSprite };
	
	Animation *lowerColumnAnim = new Animation(lowerColumnSpriteVector, 0.0f);
	Animation *upperColumnAnim = new Animation(upperColumnSpriteVector, 0.0f);

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
			separationDistanceY = mMaxVerticalSeparation;
			separationDistanceY *= mScreenScaling;
		}
		else
		{
			ColumnPair &pair = mColumns.back();

			posYLower = mMinVerticalPos + (mMaxVerticalPos - mMinVerticalPos) * (float)rand() / (float)RAND_MAX;
			posYLower *= mScreenScaling;
			
			separationDistanceY = mMinVerticalSeparation + (mMaxVerticalSeparation - mMinVerticalSeparation) * (float)rand() / (float)RAND_MAX;
			separationDistanceY *= mScreenScaling;

			posX = ComputeXPos(pair.first->mPosition.x, pair.first->mPosition.y, pair.second->mPosition.y, posYLower, posYLower + mSpriteHeight * mScreenScaling + separationDistanceY);
		}

		posLowerCol.x = posX;
		posLowerCol.y = posYLower;
		posUpperCol.x = posX;
		posUpperCol.y = posYLower + mSpriteHeight * mScreenScaling + separationDistanceY;

		ColumnGameObject *lowerColumnGO = new ColumnGameObject(posLowerCol, 0.0f, glm::vec2(mShiftSpeedX, 0.0f), lowerColumnAnim);
		ColumnGameObject *upperColumnGO = new ColumnGameObject(posUpperCol, 0.0f, glm::vec2(mShiftSpeedX, 0.0f), upperColumnAnim);

		ColumnPair columnPair = std::make_pair(lowerColumnGO, upperColumnGO);
		mColumns.push_back(columnPair);
	}
}

void GameLevel::UpdateColumnsPosition(float dt)
{
	for (ColumnPair &pair : mColumns)
	{
		pair.first->mPosition.x -= mShiftSpeedX * dt;
		pair.second->mPosition.x -= mShiftSpeedX * dt;
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

	float separationDistanceY = mMinVerticalSeparation + (mMaxVerticalSeparation - mMinVerticalSeparation) * (float)rand() / (float)RAND_MAX;
	separationDistanceY *= mScreenScaling;

	ColumnPair &lastPair = mColumns.back();

	float posX = ComputeXPos(lastPair.first->mPosition.x, lastPair.first->mPosition.y, lastPair.second->mPosition.y, posYLower, posYLower + mSpriteHeight * mScreenScaling + separationDistanceY);
	
	glm::vec2 posLowerCol(posX, posYLower);
	glm::vec2 posUpperCol(posX, posYLower + mSpriteHeight * mScreenScaling + separationDistanceY);

	columnOut.first->mPosition = posLowerCol;
	columnOut.second->mPosition = posUpperCol;

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

void GameLevel::Clear()
{
	ColumnPair &pair = mColumns.front();
	DELETE_PTR(pair.first->mAnimation);
	DELETE_PTR(pair.second->mAnimation);

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
			return posLowerLeftX + 3 * mSpriteWidth * mScreenScaling;
		}
		
		return posLowerLeftX + (rand() % 3 + 1) * mSpriteWidth * mScreenScaling;
	}
	else
	{
		if (posUpperRightY - posLowerLeft < mPassThreshold * mScreenScaling)
		{
			return posLowerLeftX + 3 * mSpriteWidth * mScreenScaling;
		}
		
		return posLowerLeftX + (rand() % 3 + 1) * mSpriteWidth * mScreenScaling;
	}
}


