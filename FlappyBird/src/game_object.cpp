#include "game_object.h"
#include "globals.h"

GameObject::GameObject(glm::vec2 position, float rotInDegrees, glm::vec2 velocity, Animation* animation) : mPosition(position),
mRotInDegrees(rotInDegrees), mVelocity(velocity), mAnimation(animation){}

GameObject::~GameObject() 
{
	Clear();
}

void GameObject::UpdatePosition(float dt){}

void GameObject::Draw(const SpriteRenderer *spriteRenderer, float dt) const
{
	spriteRenderer->DrawSprite(mAnimation->GetSprite(dt), mPosition, mRotInDegrees);
}

void GameObject::Clear()
{
	DELETE_PTR(mAnimation);
}
