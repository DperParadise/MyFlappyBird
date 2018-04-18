#ifndef __COLUMN_GAME_OBJECT_H__
#define __COLUMN_GAME_OBJECT_H__

#include "game_object.h"
#include <glm/glm.hpp>
#include "animation.h"

class ColumnGameObject final : public GameObject
{
public:
	ColumnGameObject(glm::vec2 position, float rotInDegrees, glm::vec2 velocity, Animation* animation);
	~ColumnGameObject();

	virtual void UpdatePosition(float dt);
};

#endif
