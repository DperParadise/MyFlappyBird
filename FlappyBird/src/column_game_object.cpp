#include "column_game_object.h"

ColumnGameObject::ColumnGameObject(glm::vec2 position,
	float rotInDegrees,
	glm::vec2 velocity,
	Animation* animation) : GameObject(position, rotInDegrees, velocity, animation) {}

ColumnGameObject::~ColumnGameObject() {}

void ColumnGameObject::UpdatePosition(float dt)
{
	mPosition.x += mVelocity.x * dt;
}