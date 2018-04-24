#ifndef __COLUMN_GAME_OBJECT_H__
#define __COLUMN_GAME_OBJECT_H__

#include "game_object.h"
#include <glm/glm.hpp>
#include "animation.h"

class BirdGameObject;
class Game;
class ColumnGameObject final : public GameObject
{
public:
	ColumnGameObject(glm::vec2 position, float rotInDegrees, glm::vec2 velocity, Animation* animation, const BirdGameObject *bird, Game *game, float screenScaling, bool isLowerColumn);
	~ColumnGameObject();

	virtual void UpdatePosition(float dt);

	void ResetScoreComputed();


private:
	bool mScoreComputed = false;
	const BirdGameObject *mBird = nullptr;
	Game *mGame = nullptr;
	float mScreenScaling;
	bool mIsLowerColumn = false;
};

#endif
