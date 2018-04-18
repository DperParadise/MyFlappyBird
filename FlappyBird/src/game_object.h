#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <glm/glm.hpp>
#include "animation.h"
#include "sprite_renderer.h"

class GameObject
{
public:
	glm::vec2 mPosition;
	float mRotInDegrees;
	glm::vec2 mVelocity;
	Animation *mAnimation = nullptr;
	
	GameObject(glm::vec2 position, float rotInDegrees, glm::vec2 velocity, Animation* animation);
	virtual ~GameObject();
	virtual void UpdatePosition(float dt);
	void Draw(const SpriteRenderer *spriteRenderer, float dt) const;
};


#endif
