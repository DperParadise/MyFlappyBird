#ifndef __BIRD_GAME_OBJECT_H__
#define __BIRD_GAME_OBJECT_H__

#include "game_object.h"
#include <glm/glm.hpp>
#include "animation.h"

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
};
#endif
