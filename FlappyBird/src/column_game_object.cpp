#include "column_game_object.h"
#include "bird_game_object.h"
#include "game.h"

ColumnGameObject::ColumnGameObject(glm::vec2 position,
	float rotInDegrees,
	glm::vec2 velocity,
	Animation* animation,
	const BirdGameObject *bird,
	Game *game,
	float screenScaling,
	bool isLowerColumn) : mBird(bird), mGame(game), mScreenScaling(screenScaling), mIsLowerColumn(isLowerColumn), GameObject(position, rotInDegrees, velocity, animation) {}

ColumnGameObject::~ColumnGameObject() {}

void ColumnGameObject::UpdatePosition(float dt)
{
	mPosition.x += mVelocity.x * dt;
	
	//check if bird scored
	if (mIsLowerColumn)
	{
		float columnRightEdge = mPosition.x + mAnimation->mSprites[0]->GetWidth() * mScreenScaling;
		if (columnRightEdge < mBird->mPosition.x && !mScoreComputed && mBird->IsAlive())
		{
			mScoreComputed = true;
			mGame->ComputeScore();
		}
	}
}

void ColumnGameObject::ResetScoreComputed()
{
	mScoreComputed = false;
}

