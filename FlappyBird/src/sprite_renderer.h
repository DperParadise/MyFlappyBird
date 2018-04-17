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
	SpriteRenderer(const Shader &shader);
	~SpriteRenderer();
	void DrawSprite(const Sprite *sprite, const glm::vec2 &position, float rotInDegrees, const glm::vec2 &size, bool isStaticDraw) const;

private:
	Shader mShader;
	GLuint quadVAOStaticDraw;
	GLuint quadVAODynamicDraw;
	GLuint quadVBODynamic;

	const int numTexCoords = 6;
	float *mTexCoords;
	void Init();
};





#endif
