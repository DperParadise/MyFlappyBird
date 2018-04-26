#include "bird_game_object.h"
#include <glm/glm.hpp>
#include "globals.h"
#include "resource_manager.h"

BirdGameObject::BirdGameObject(glm::vec2 position,
	float rotInDegrees,
	glm::vec2 velocity,
	float acceleration,
	Animation* animation) : mAcceleration(acceleration), GameObject(position, rotInDegrees, velocity, animation) 
{
	LoadProperties();
}

BirdGameObject::~BirdGameObject() {}

void BirdGameObject::UpdatePosition(float dt)
{
	mVelocity.y += mAcceleration * dt;
	mPosition.x += mVelocity.x * dt;
	mPosition.y += mVelocity.y * dt;
	mFallingTime += dt;

	if (mJumpPressed)
	{
		mVelocity.y += mJumpVelocityFactor * mAcceleration;
		mJumpPressed = false;
		mFallingTime = 0.0f;  
		mFalling = false;
	}

	mVelocity.y = glm::clamp(mVelocity.y, mMinVelocity, mMaxVelocity);
	
	if (mFalling && mFallingTime >= mTimeToStartRotation)
	{
		mRotInDegrees =  mMinRotation * (mFallingTime - mTimeToStartRotation) / mTimeToMinRotation;
		mRotInDegrees = glm::clamp(mRotInDegrees, mMinRotation, mMaxRotation);
	}
	else if(!mFalling)
	{
		mRotInDegrees += mRotSpeeedNotFalling * dt;
		if (mRotInDegrees >= mMaxRotation)
		{
			mRotInDegrees = mMaxRotation;
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

void BirdGameObject::LoadProperties()
{
	mMinVelocity = ResourceManager::GetPropFloat("BirdGameObject.MinVelocity");
	mMaxVelocity = ResourceManager::GetPropFloat("BirdGameObject.MaxVelocity");
	mMinRotation = ResourceManager::GetPropFloat("BirdGameObject.MinRotation");
	mMaxRotation = ResourceManager::GetPropFloat("BirdGameObject.MaxRotation");
	mTimeToStartRotation = ResourceManager::GetPropFloat("BirdGameObject.TimeToStartRotation");
	mTimeToMinRotation = ResourceManager::GetPropFloat("BirdGameObject.TimeToMinRotation");
	mRotSpeeedNotFalling = ResourceManager::GetPropFloat("BirdGameObject.RotSpeeedNotFalling");
}