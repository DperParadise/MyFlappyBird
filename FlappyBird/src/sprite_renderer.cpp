#include "sprite_renderer.h"
#include <glm/gtx/transform.hpp>

SpriteRenderer::SpriteRenderer(const Shader &shader) : mShader(shader){
	Init();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &quadVAOStaticDraw);
	glDeleteVertexArrays(1, &quadVAODynamicDraw);
	delete[] mTexCoords;
}

void SpriteRenderer::DrawSprite(const Sprite *sprite, const glm::vec2 &position, float rotInDegrees, const glm::vec2 &size, bool isStaticDraw)
{
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));

	//Rotate the sprite around the center of the quad
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians<float>(rotInDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	
	model = glm::scale(model, glm::vec3(size, 1.0f));

	mShader.Use();
	mShader.SetMatrix4("model", model);

	glActiveTexture(GL_TEXTURE0);
	sprite->mTexture.Bind();

	if (isStaticDraw)
	{
		glBindVertexArray(quadVAOStaticDraw);
	}
	else
	{
		glBindVertexArray(quadVAODynamicDraw);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBODynamic);
		glBufferSubData(GL_ARRAY_BUFFER, 2 * numTexCoords * sizeof(float), 2 * numTexCoords * sizeof(float), sprite->texCoords);
	}

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
	glGenVertexArrays(1, &quadVAOStaticDraw);
	glBindVertexArray(quadVAOStaticDraw);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(texCoords), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(texCoords), texCoords);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(vertices)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//Dynamic data for sprites with different animations and texture coordinates
	mTexCoords = new float[2 * numTexCoords] {	0.0f, 1.0f,
												1.0f, 1.0f,
												0.0f, 0.0f,
												1.0f, 1.0f,
												1.0f, 0.0f,
												0.0f, 0.0f
	};
	glGenVertexArrays(1, &quadVAODynamicDraw);
	glBindVertexArray(quadVAODynamicDraw);
	glGenBuffers(1, &quadVBODynamic);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBODynamic);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + 2 * numTexCoords * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), 2 * numTexCoords * sizeof(float), mTexCoords);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(vertices)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

}
