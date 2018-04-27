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
	void DrawSprite(const Sprite *sprite, const glm::vec2 &position, float rotInDegrees, float alpha = 1.0f) const;
	void DrawSpriteShifted(const Sprite *sprite, const glm::vec2 &position, float deltaShift) const;
	bool DrawSpriteFade(const Sprite *sprite, const glm::vec2 &position, bool IsFadingIn, float dt);
	void ResetAlpha();

private:
	Shader mShader;
	GLuint mQuadVAO;
	GLuint mQuadVBO;
	float mScreenScaling;
	const int mNumTexCoords = 6;
	float mFadeSpeed = 5.0f;
	float mAlpha = 0.0f;

	void Init();
};





#endif
