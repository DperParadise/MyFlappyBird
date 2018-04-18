#include "game_object.h"

GameObject::GameObject(glm::vec2 position, float rotInDegrees, glm::vec2 velocity, Animation* animation) : mPosition(position),
mRotInDegrees(rotInDegrees), mVelocity(velocity), mAnimation(animation){}

GameObject::~GameObject() {}

void GameObject::UpdatePosition(float dt){}

void GameObject::Draw(const SpriteRenderer *spriteRenderer, float dt) const
{
	spriteRenderer->DrawSprite(mAnimation->GetSprite(dt), mPosition, mRotInDegrees);
}
