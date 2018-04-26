#ifndef __BIRD_GAME_OBJECT_H__
#define __BIRD_GAME_OBJECT_H__

#include "game_object.h"
#include <glm/glm.hpp>
#include "animation.h"
#include <string>

class BirdGameObject final : public GameObject
{
public:
	float mWorldGravity;
	bool mJumpPressed = false;

	BirdGameObject(glm::vec2 position, float rotInDegrees, glm::vec2 velocity, int worldGravity, Animation* animation, float screenScaling);
	~BirdGameObject();

	virtual void UpdatePosition(float dt);
	void Hover(float dt);
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
	float mHoverSpeed = 8.0f;
	int mMaxHoverAmp = 20;
	float mTimer = 0.0f;
	float mMaxTimerCount = 60.0f;
	float mScreenScaling;
	void LoadProperties();
};
#endif
