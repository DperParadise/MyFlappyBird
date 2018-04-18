#include "sprite_renderer.h"
#include <glm/gtx/transform.hpp>

SpriteRenderer::SpriteRenderer(const Shader &shader, float screenScaling) : mShader(shader), mScreenScaling(screenScaling){
	Init();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteBuffers(1, &quadVBO);
	glDeleteVertexArrays(1, &quadVAO);
}

void SpriteRenderer::DrawSprite(const Sprite *sprite, const glm::vec2 &position, float rotInDegrees) const
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

	glActiveTexture(GL_TEXTURE0);
	sprite->mTexture.Bind();
	
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * numTexCoords * sizeof(float), 2 * numTexCoords * sizeof(float), sprite->texCoords);
	glBindVertexArray(quadVAO);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void SpriteRenderer::Init()
{
	//Static data for sprites without animations
		
	float vertices[] = { 0.0f, 0.0f,
						1.0f, 0.0f,
						0.0f, 1.0f,
						1.0f, 0.0f,
						1.0f, 1.0f,
						0.0f, 1.0
	};

	float texCoords[] = {	0.0f, 1.0f,
							1.0f, 1.0f,
							0.0f, 0.0f,
							1.0f, 1.0f,
							1.0f, 0.0f,
							0.0f, 0.0f
	};
	
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + 2 * numTexCoords * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), 2 * numTexCoords * sizeof(float), texCoords);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(vertices)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

}
