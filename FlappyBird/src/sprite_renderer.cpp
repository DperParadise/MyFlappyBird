#include "sprite_renderer.h"
#include <glm/gtx/transform.hpp>

SpriteRenderer::SpriteRenderer(const Shader &shader) : mShader(shader){
	Init();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &quadVAO);
}

void SpriteRenderer::DrawSprite(const Texture2D &texture, const glm::vec2 &position, float rotInDegrees, const glm::vec2 &size) const
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
	texture.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void SpriteRenderer::Init()
{
						//vertices	//text coord
	float vertices[] = { 0.0f, 0.0f, 0.0f, 1.0f,
						1.0f, 0.0f,	1.0f, 1.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						1.0f, 0.0f, 1.0f, 1.0f,
						1.0f, 1.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f
	};
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
