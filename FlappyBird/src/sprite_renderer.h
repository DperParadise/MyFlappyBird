#ifndef __SPRITE_RENDERER_H__
#define __SPRITE_RENDERER_H__

#include "shader.h"
#include <glad/glad.h>
#include "texture2D.h"
#include <glm/glm.hpp>
#include "sprite.h"

class SpriteRenderer final
{
public:
	SpriteRenderer(const Shader &shader, float screenScaling);
	~SpriteRenderer();
	void DrawSprite(const Sprite *sprite, const glm::vec2 &position, float rotInDegrees) const;
	void DrawSpriteShifted(const Sprite *sprite, const glm::vec2 &position, float rotInDegrees, float deltaShift) const;

private:
	Shader mShader;
	GLuint mQuadVAO;
	GLuint mQuadVBO;
	float mScreenScaling;

	const int mNumTexCoords = 6;
	void Init();
};





#endif
