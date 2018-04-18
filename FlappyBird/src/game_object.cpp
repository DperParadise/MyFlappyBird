#include "game_object.h"

GameObject::GameObject(glm::vec2 position, float rotInDegrees, glm::vec2 velocity, float acceleration, Animation* animation) : mPosition(position),
mRotInDegrees(rotInDegrees), mVelocity(velocity), mAcceleration(acceleration), mAnimation(animation){}


void GameObject::UpdatePosition(float dt)
{
	mVelocity.y += mAcceleration * dt;

	mPosition.x += mVelocity.x * dt;
	mPosition.y += mVelocity.y * dt;
}

void GameObject::Draw(const SpriteRenderer *spriteRenderer, float dt) const
{
	spriteRenderer->DrawSprite(mAnimation->GetSprite(dt), mPosition, mRotInDegrees);
}
