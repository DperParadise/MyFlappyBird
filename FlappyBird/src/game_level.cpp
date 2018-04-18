#include "game_level.h"
#include <glm/glm.hpp>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include "sprite.h"
#include "animation.h"
#include "game_object.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

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
	ColumnPair &pair = columns.front();
	if (pair.first->mAnimation != nullptr)
	{
		pair.first->mAnimation->Clear();
		delete pair.first->mAnimation;
		pair.first->mAnimation = nullptr;
	}
	
	if (pair.second->mAnimation != nullptr)
	{
		pair.second->mAnimation->Clear();
		delete pair.second->mAnimation;
		pair.second->mAnimation = nullptr;
	}
	
}

void GameLevel::Init()
{
	spriteWidth = 26;
	spriteHeight = 160;

	Sprite *lowerColumnSprite = new Sprite(ResourceManager::GetTexture("flappyBirdAtlas"), 84, 323, spriteWidth, spriteHeight);
	Sprite *upperColumnSprite = new Sprite(ResourceManager::GetTexture("flappyBirdAtlas"), 56, 323, spriteWidth, spriteHeight);
	
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

	for (int i = 0; i < numColumns; i++)
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
			ColumnPair &pair = columns.back();
			posX = pair.first->mPosition.x + (rand() % 3 + 1) * spriteWidth * mScreenScaling;  
			
			posYLower = mMinVerticalPos + (mMaxVerticalPos - mMinVerticalPos) * (float)rand() / (float)RAND_MAX;
			posYLower *= mScreenScaling;
			
			separationDistanceY = mMinVerticalSeparation + (mMaxVerticalSeparation - mMinVerticalSeparation) * (float)rand() / (float)RAND_MAX;
			separationDistanceY *= mScreenScaling;
		}

		posLowerCol.x = posX;
		posLowerCol.y = posYLower;
		posUpperCol.x = posX;
		posUpperCol.y = posYLower + spriteHeight * mScreenScaling + separationDistanceY;

		GameObject *lowerColumnGO = new GameObject(posLowerCol, 0.0f, glm::vec2(mShiftSpeedX, 0.0f), 0.0f, lowerColumnAnim);
		GameObject *upperColumnGO = new GameObject(posUpperCol, 0.0f, glm::vec2(mShiftSpeedX, 0.0f), 0.0f, upperColumnAnim);

		ColumnPair columnPair = std::make_pair(lowerColumnGO, upperColumnGO);
		columns.push_back(columnPair);
	}
}

void GameLevel::UpdateColumnsPosition(float dt)
{
	for (ColumnPair &pair : columns)
	{
		pair.first->mPosition.x -= mShiftSpeedX * dt;
		pair.second->mPosition.x -= mShiftSpeedX * dt;
	}

	ColumnPair &firstPair = columns.front();

	//Sprite is out of the screen boundaries
	if (firstPair.first->mPosition.x + spriteWidth * mScreenScaling < 0.0f)
	{
		ResetColumnPairPosition();
	}
}

void GameLevel::ResetColumnPairPosition()
{
	ColumnPair columnOut = columns.front();
	columns.pop_front();

	ColumnPair &lastPair = columns.back();
	float posX = lastPair.first->mPosition.x + (rand() % 3 + 1) * spriteWidth * mScreenScaling;

	float posYLower = mMinVerticalPos + (mMaxVerticalPos - mMinVerticalPos) * (float)rand() / (float)RAND_MAX;
	posYLower *= mScreenScaling;

	float separationDistanceY = mMinVerticalSeparation + (mMaxVerticalSeparation - mMinVerticalSeparation) * (float)rand() / (float)RAND_MAX;
	separationDistanceY *= mScreenScaling;

	glm::vec2 posLowerCol(posX, posYLower);
	glm::vec2 posUpperCol(posX, posYLower + spriteHeight * mScreenScaling + separationDistanceY);

	columnOut.first->mPosition = posLowerCol;
	columnOut.second->mPosition = posUpperCol;

	columns.push_back(columnOut);
}

void GameLevel::DrawLevel(float dt) const
{
	for (const ColumnPair &pair : columns)
	{
		
		if (pair.first->mPosition.x > mScreenWidth)
		{
			break;
		}
		
		pair.first->Draw(mSpriteRenderer, dt);
		pair.second->Draw(mSpriteRenderer, dt);
	}
}



