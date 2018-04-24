#ifndef __BIRD_GAME_OBJECT_H__
#define __BIRD_GAME_OBJECT_H__

#include "game_object.h"
#include <glm/glm.hpp>
#include "animation.h"
#include <string>

class BirdGameObject final : public GameObject
{
public:
	float mAcceleration;
	bool mJumpPressed = false;

	BirdGameObject(glm::vec2 position, float rotInDegrees, glm::vec2 velocity, float acceleration, Animation* animation);
	~BirdGameObject();

	virtual void UpdatePosition(float dt);
	bool IsAlive() const;
	void Kill();
	void SetAlive();

private:
	float mFallingTime = 0.0f;
	bool mFalling = true;
	bool mAlive = true;
	float mJumpVelocityFactor = -2.0f;
	float mMinVelocity = -500.0f;
	float mMaxVelocity = 500.0f;
	float mMinRotation = -90.0f;
	float mMaxRotation = 30.0f;
	float mTimeToStartRotation = 0.5f;
	float mTimeToMinRotation = 0.3f;
	float mRotSpeeedNotFalling = 1000.0f;

	void LoadProperties();
};
#endif
