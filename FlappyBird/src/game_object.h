#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <glm/glm.hpp>
#include "animation.h"
#include "sprite_renderer.h"
#include <vector>

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
	void PrepareShake();
	void BeginShake();
	void EndShake();

private:
	int mShakeValue = 3;
	glm::vec2 mPosBeforeShake;

	void Clear();
};


#endif
