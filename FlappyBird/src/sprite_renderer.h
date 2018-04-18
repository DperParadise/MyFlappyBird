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

private:
	Shader mShader;
	GLuint quadVAO;
	GLuint quadVBO;
	float mScreenScaling;

	const int numTexCoords = 6;
	void Init();
};





#endif
