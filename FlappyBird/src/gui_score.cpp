#include "gui_score.h"
#include "resource_manager.h"
#include "globals.h"
#include "sprite_renderer.h"

GUIScore::GUIScore(int screenWidth, int screenHeight, float screenScaling) : mScreenWidth(screenWidth), mScreenHeight(screenHeight), mScreenScaling(screenScaling)
{
	Init();
}

GUIScore::~GUIScore()
{
	Clear();
}

void GUIScore::Draw(unsigned int score, const SpriteRenderer *renderer) const
{
	std::string scoreString = std::to_string(score);
	int totalLength = 0;
	for (int i = 0; i < scoreString.size(); i++)
	{
		totalLength += mNumbers.find(scoreString[i])->second->GetWidth();
	}

	float posX = mScreenWidth * mFactorScreenWidth - totalLength * mFactorScreenWidth * mScreenScaling;
	float posY = mScreenHeight - mFactorScreenHeight * mScreenHeight;
	for (int i = 0; i < scoreString.size(); i++)
	{
		Sprite *sprite = mNumbers.find(scoreString[i])->second;
		int offset = sprite->GetWidth();
		renderer->DrawSprite(sprite, glm::vec2(posX, posY), 0.0f);
		posX += offset * mScreenScaling;
	}
}

void GUIScore::Init()
{
	mFactorScreenWidth = ResourceManager::GetPropFloat("GUIScore.mFactorScreenWidth");
	mFactorScreenHeight = ResourceManager::GetPropFloat("GUIScore.mFactorScreenHeight");

	int originY = ResourceManager::GetPropInt("GUIScore.mSpriteOriginY");
	mSpriteWidth = ResourceManager::GetPropInt("GUIScore.mNumWidth");
	int height = ResourceManager::GetPropInt("GUIScore.mNumHeight");
	std::string textureName = ResourceManager::GetPropString("Game.mFlappyBirdSpriteAtlasName");

	std::string propOriginX = "GUIScore.mNum1SpriteOriginX";
	std::string propSpriteNum1Width = "GUIScore.mNum1Width";
	int spriteNum1Width = ResourceManager::GetPropInt(propSpriteNum1Width);
	Sprite *sprite = new Sprite(ResourceManager::GetTexture(textureName), ResourceManager::GetPropInt(propOriginX), originY, spriteNum1Width, height);
	mNumbers.insert(std::make_pair('1', sprite));
	
	for (int i = 0; i < 10; i++)
	{
		if (i == 1)
			continue;

		std::string prop = "GUIScore.mNum" + std::to_string(i) + "SpriteOriginX";
		sprite = new Sprite(ResourceManager::GetTexture(textureName), ResourceManager::GetPropInt(prop), originY, mSpriteWidth, height);
		mNumbers.insert(std::make_pair(std::to_string(i)[0], sprite));
	}
}

void GUIScore::Clear()
{
	for (auto pair : mNumbers)
	{
		DELETE_PTR(pair.second);
	}
	mNumbers.clear();
}

