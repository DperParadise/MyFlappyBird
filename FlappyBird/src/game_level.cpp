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

GameLevel::GameLevel(int screenWidth, int screenHeight, float screenScaling, float minVerticalSeparation, float maxVerticalSeparation, float minVerticalPos, float maxVerticalPos, float shiftSpeedX, const SpriteRenderer *spriteRenderer) :
	mScreenWidth(screenWidth),
	mScreenHeight(screenHeight),
	mScreenScaling(screenScaling),
	mMinVerticalSeparation(minVerticalSeparation),
	mMaxVerticalSeparation(maxVerticalSeparation),
	mMinVerticalPos(minVerticalPos), 
	mMaxVerticalPos(maxVerticalPos),
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

	for (int i = 0; i < mNumColumns; i++)
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
			posX = pair.first->mPosition.x + (rand() % 3 + 1) * mSpriteWidth * mScreenScaling;  
			
			posYLower = mMinVerticalPos + (mMaxVerticalPos - mMinVerticalPos) * (float)rand() / (float)RAND_MAX;
			posYLower *= mScreenScaling;
			
			separationDistanceY = mMinVerticalSeparation + (mMaxVerticalSeparation - mMinVerticalSeparation) * (float)rand() / (float)RAND_MAX;
			separationDistanceY *= mScreenScaling;
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

	ColumnPair &lastPair = mColumns.back();
	float posX = lastPair.first->mPosition.x + (rand() % 3 + 1) * mSpriteWidth * mScreenScaling;

	float posYLower = mMinVerticalPos + (mMaxVerticalPos - mMinVerticalPos) * (float)rand() / (float)RAND_MAX;
	posYLower *= mScreenScaling;

	float separationDistanceY = mMinVerticalSeparation + (mMaxVerticalSeparation - mMinVerticalSeparation) * (float)rand() / (float)RAND_MAX;
	separationDistanceY *= mScreenScaling;

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
	DELETE(pair.first->mAnimation);
	DELETE(pair.second->mAnimation);

	mColumns.clear();
}



