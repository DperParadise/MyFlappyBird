#include "game.h"
#include "resource_manager.h"
#include "bird_game_object.h"
#include "game_level.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "sprite_renderer.h"
#include "sprite.h"
#include "animation.h"
#include <GLFW/glfw3.h>
#include "globals.h"

Game::Game(int screenWidth, int screenHeight, float screenScaling) : mScreenWidth(screenWidth), mScreenHeight(screenHeight), mScreenScaling(screenScaling) {}

Game::~Game()
{
	Clear();
}

void Game::Init()
{
	memset(mKeys, 0, 1024 * sizeof(bool));
	memset(mKeysProcessed, 0, 1024 * sizeof(bool));

	ResourceManager::LoadTexture("flappyBirdSpriteAtlas", "resources/textures/flappy_bird_sprite_sheet.png");
	ResourceManager::LoadShader("testShader", "resources/shaders/test.vs", "resources/shaders/test.fs");
	glm::mat4 projection;
	projection = glm::ortho(0.0f, float(mScreenWidth), 0.0f, float(mScreenHeight), -1.0f, 1.0f);
	ResourceManager::GetShader("testShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("testShader").SetInteger("textureSampler", 0);

	mRenderer = new SpriteRenderer(ResourceManager::GetShader("testShader"), 3.0f);

	mLevel = new GameLevel(mScreenWidth, mScreenHeight, 3.0f, 64.0f, 64.0f, -150.0f, 150.0f, 100.f, mRenderer);

	std::vector<Sprite*> flySprites;
	flySprites.push_back(new Sprite(ResourceManager::GetTexture("flappyBirdSpriteAtlas"), 3, 491, 17, 12));
	flySprites.push_back(new Sprite(ResourceManager::GetTexture("flappyBirdSpriteAtlas"), 31, 491, 17, 12));
	flySprites.push_back(new Sprite(ResourceManager::GetTexture("flappyBirdSpriteAtlas"), 59, 491, 17, 12));
	Animation *flyAnimation = new Animation(flySprites, 10.0);
	mFlappyBird = new BirdGameObject(glm::vec2(0.0f, mScreenHeight), 0.0f, glm::vec2(0.0f, 0.0f), -333.0f * mScreenScaling, flyAnimation);
}

void Game::ProcessInput()
{
	if (mKeys[GLFW_KEY_SPACE] && !mKeysProcessed[GLFW_KEY_SPACE])
	{
		mFlappyBird->mJumpPressed = true;
		mKeysProcessed[GLFW_KEY_SPACE] = true;
	}
}

void Game::Update(float dt)
{
	mFlappyBird->UpdatePosition(dt);
	mLevel->UpdateColumnsPosition(dt);

	DoCollissions();
}

void Game::Render(float dt)
{
	mLevel->DrawLevel(dt);
	mFlappyBird->Draw(mRenderer, dt);
}

void Game::DoCollissions()
{

}

void Game::CheckCollissions(BirdGameObject *one, ColumnGameObject *two)
{

}

void Game::Clear()
{
	DELETE_PTR(mRenderer);
}