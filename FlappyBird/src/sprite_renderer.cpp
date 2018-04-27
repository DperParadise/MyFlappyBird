#include "sprite_renderer.h"
#include <glm/gtx/transform.hpp>

SpriteRenderer::SpriteRenderer(const Shader &shader, float screenScaling) : mShader(shader), mScreenScaling(screenScaling){
	Init();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteBuffers(1, &mQuadVBO);
	glDeleteVertexArrays(1, &mQuadVAO);
}

void SpriteRenderer::DrawSprite(const Sprite *sprite, const glm::vec2 &position, float rotInDegrees, float alpha) const
{
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));

	//Rotate the sprite around the center of the quad
	model = glm::translate(model, glm::vec3(0.5f * sprite->GetWidth() * mScreenScaling, 0.5f * sprite->GetHeight() * mScreenScaling, 0.0f));
	model = glm::rotate(model, glm::radians<float>(rotInDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * sprite->GetWidth() * mScreenScaling, -0.5f * sprite->GetHeight() * mScreenScaling, 0.0f));
	
	model = glm::scale(model, glm::vec3(sprite->GetWidth() * mScreenScaling, sprite->GetHeight() * mScreenScaling, 1.0f));

	mShader.Use();
	mShader.SetMatrix4("model", model);
	mShader.SetFloat("alpha", alpha);

	glActiveTexture(GL_TEXTURE0);
	sprite->mTexture.Bind();
	
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * mNumTexCoords * sizeof(float), 2 * mNumTexCoords * sizeof(float), sprite->mTexCoords);
	glBindVertexArray(mQuadVAO);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void SpriteRenderer::DrawSpriteShifted(const Sprite *sprite, const glm::vec2 &position, float deltaShift) const
{
	for (int i = 0; i < 2 * mNumTexCoords; i += 2)
	{
		sprite->mTexCoords[i] += deltaShift / (mScreenScaling * sprite->mTexture.GetWidth());
	}
	
	DrawSprite(sprite, position, 0.0f);
}

void SpriteRenderer::DrawSpriteFade(const Sprite *sprite, const glm::vec2 &position, bool IsFadingIn, float dt)
{
	if (IsFadingIn)
	{
		mAlpha += mFadeSpeed * dt;
		if (mAlpha > 1.0f)
		{
			mAlpha = 1.0f;
		}
	}
	else
	{
		mAlpha -= mFadeSpeed * dt;
		if (mAlpha < 0.0f)
		{
			mAlpha = 0.0f;
		}
	}
	DrawSprite(sprite, position, 0.0f, mAlpha);
}

void SpriteRenderer::Init()
{
	float vertices[] = { 0.0f, 0.0f,
						1.0f, 0.0f,
						0.0f, 1.0f,
						1.0f, 0.0f,
						1.0f, 1.0f,
						0.0f, 1.0
	};
	
	
	float texCoords[] = { 0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0
	};

	glGenVertexArrays(1, &mQuadVAO);
	glBindVertexArray(mQuadVAO);
	glGenBuffers(1, &mQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + 2 * mNumTexCoords * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), 2 * mNumTexCoords * sizeof(float), texCoords);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(vertices)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

}
