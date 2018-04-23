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
#include "column_game_object.h"

//TEST
#include <iostream>

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
	ResourceManager::LoadTexture("foreground", "resources/textures/foreground.png");
	ResourceManager::LoadShader("testShader", "resources/shaders/test.vs", "resources/shaders/test.fs");
	glm::mat4 projection;
	projection = glm::ortho(0.0f, float(mScreenWidth), 0.0f, float(mScreenHeight), -1.0f, 1.0f);
	ResourceManager::GetShader("testShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("testShader").SetInteger("textureSampler", 0);

	mRenderer = new SpriteRenderer(ResourceManager::GetShader("testShader"), 3.0f);
																							//threshold
	mLevel = new GameLevel(mScreenWidth, mScreenHeight, 3.0f, 64.0f, 64.0f, -104.0f, 24.0f, 3 * 12.0f, 100.0f, mRenderer);

	std::vector<Sprite*> flySprites;
	flySprites.push_back(new Sprite(ResourceManager::GetTexture("flappyBirdSpriteAtlas"), 3, 491, 17, 12));
	flySprites.push_back(new Sprite(ResourceManager::GetTexture("flappyBirdSpriteAtlas"), 31, 491, 17, 12));
	flySprites.push_back(new Sprite(ResourceManager::GetTexture("flappyBirdSpriteAtlas"), 59, 491, 17, 12));
	Animation *flyAnimation = new Animation(flySprites, 10.0);
	mFlappyBird = new BirdGameObject(glm::vec2(0.0f, mScreenHeight), 0.0f, glm::vec2(0.0f, 0.0f), -333.0f * mScreenScaling, flyAnimation);

	mBackground = new Sprite(ResourceManager::GetTexture("flappyBirdSpriteAtlas"), 0, 0, 144, 256);
	
	mForeground = new Sprite(ResourceManager::GetTexture("foreground"), 0, 0, 168, 56);
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
	mRenderer->DrawSprite(mBackground, glm::vec2(0.0f), 0.0f);
	mLevel->DrawLevel(dt);
	mFlappyBird->Draw(mRenderer, dt);
	mRenderer->DrawSpriteShifted(mForeground, glm::vec2(0.0f), 0.0f, 60.0f * dt);
}

void Game::DoCollissions()
{
	CheckCollissions(mFlappyBird, mForeground->GetHeight());

	for (int i = 0; i < mLevel->mNumColumnPairs; i++)
	{
		if (mLevel->mColumns[i].first->mPosition.x > mScreenWidth * mScreenScaling)
		{
			break;
		}
		CheckCollissions(mFlappyBird, mLevel->mColumns[i].first);
		CheckCollissions(mFlappyBird, mLevel->mColumns[i].second);
	}
}

void Game::CheckCollissions(BirdGameObject *bird, ColumnGameObject *column)
{
	Sprite *birdSprite = bird->mAnimation->mSprites[0];
	glm::vec2 birdCentre = bird->mPosition + glm::vec2(birdSprite->GetWidth() * 0.5f * mScreenScaling, birdSprite->GetHeight() * 0.5f * mScreenScaling);

	Sprite *colSprite = column->mAnimation->mSprites[0];
	glm::vec2 colCentre = column->mPosition + glm::vec2(colSprite->GetWidth() * 0.5f * mScreenScaling, colSprite->GetHeight() * 0.5f * mScreenScaling);

	glm::vec2 colToBirdVector = birdCentre - colCentre;

	glm::vec2 minHalfExtents(-colSprite->GetWidth() * 0.5f * mScreenScaling, -colSprite->GetHeight() * 0.5f * mScreenScaling);
	glm::vec2 maxHalfExtents(colSprite->GetWidth() * 0.5f * mScreenScaling, colSprite->GetHeight() * 0.5f * mScreenScaling);
	glm::vec2 closestPoint = colCentre + glm::clamp(colToBirdVector, minHalfExtents, maxHalfExtents);
	
	if (glm::length(birdCentre - closestPoint) < birdSprite->GetHeight() * mScreenScaling)
	{
		//std::cout << "Collision with column" << std::endl;
	}
}

void Game::CheckCollissions(BirdGameObject *bird, int groundHeight)
{
	if (bird->mPosition.y < groundHeight * mScreenScaling)
	{
		//std::cout << "Collission with ground" << std::endl;
	}
}

void Game::Clear()
{
	DELETE_PTR(mRenderer);
	DELETE_PTR(mFlappyBird);
	DELETE_PTR(mBackground);
	DELETE_PTR(mForeground);
}