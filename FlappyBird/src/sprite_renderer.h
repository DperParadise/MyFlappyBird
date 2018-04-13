#ifndef __SPRITE_RENDERER_H__
#define __SPRITE_RENDERER_H__

#include "shader.h"
#include <glad/glad.h>
#include "texture2D.h"
#include <glm/glm.hpp>

class SpriteRenderer final
{
public:
	SpriteRenderer(const Shader &shader);
	~SpriteRenderer();
	void DrawSprite(const Texture2D &texture, const glm::vec2 &position, float rotInDegrees, const glm::vec2 &size) const;

private:
	const Shader &mShader;
	GLuint quadVAO;
	void Init();
};





#endif
