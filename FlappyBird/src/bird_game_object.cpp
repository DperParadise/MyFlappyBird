#include "bird_game_object.h"
#include <glm/glm.hpp>
#include "globals.h"

BirdGameObject::BirdGameObject(glm::vec2 position,
	float rotInDegrees,
	glm::vec2 velocity,
	float acceleration,
	Animation* animation) : mAcceleration(acceleration), GameObject(position, rotInDegrees, velocity, animation) {}

BirdGameObject::~BirdGameObject() {}

void BirdGameObject::UpdatePosition(float dt)
{
	mVelocity.y += mAcceleration * dt;
	mPosition.x += mVelocity.x * dt;
	mPosition.y += mVelocity.y * dt;
	mFallingTime += dt;

	if (mJumpPressed)
	{
		mVelocity.y += 2.0f * -mAcceleration;
		mJumpPressed = false;
		mFallingTime = 0.0f;  
		mFalling = false;
	}

	mVelocity.y = glm::clamp(mVelocity.y, -500.0f, 500.0f);
	
	if (mFalling && mFallingTime >= 0.5f)
	{
		mRotInDegrees =  -90.0f * (mFallingTime - 0.5f) / 0.3f;
		mRotInDegrees = glm::clamp(mRotInDegrees, -90.0f, 30.0f);
	}
	else if(!mFalling)
	{
		mRotInDegrees += 1000.0f * dt;
		if (mRotInDegrees >= 30.0f)
		{
			mRotInDegrees = 30.0f;
			mFalling = true;
		}
	}	
}

bool BirdGameObject::IsAlive() const
{
	return mAlive;
}

void BirdGameObject::Kill()
{
	mAlive = false;
}

void BirdGameObject::SetAlive()
{
	mAlive = true;
}