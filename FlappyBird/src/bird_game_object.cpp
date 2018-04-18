#include "bird_game_object.h"

BirdGameObject::BirdGameObject(glm::vec2 position,
	float rotInDegrees,
	glm::vec2 velocity,
	float acceleration,
	Animation* animation) : mAcceleration(acceleration), GameObject(position, rotInDegrees, velocity, animation) {}

BirdGameObject::~BirdGameObject() {}

void BirdGameObject::UpdatePosition(float dt)
{
	mVelocity.y += mAcceleration * dt;

	mPosition.x += mVelocity.x * dt;
	mPosition.y += mVelocity.y * dt;
}