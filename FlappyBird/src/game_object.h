#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <glm/glm.hpp>
#include "animation.h"
#include "sprite_renderer.h"

class GameObject final
{
public:
	GameObject(glm::vec2 position, float rotInDegrees, glm::vec2 size, glm::vec2 velocity, float acceleration, Animation* animation, bool isStatic);

	glm::vec2 mPosition;
	float mRotInDegrees;
	glm::vec2 mSize;
	glm::vec2 mVelocity;
	float mAcceleration;
	Animation *mAnimation = nullptr;
	bool mIsStatic;

	void UpdatePosition(float dt);
	void Draw(const SpriteRenderer *spriteRenderer, float dt) const;
};


#endif
