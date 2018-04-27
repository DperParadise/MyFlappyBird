#include "gui_score.h"
#include "resource_manager.h"
#include "globals.h"
#include "sprite_renderer.h"
#include <fstream>
#include <iostream>

GUIScore::GUIScore(int screenWidth, int screenHeight, float screenScaling) : mScreenWidth(screenWidth), mScreenHeight(screenHeight), mScreenScaling(screenScaling)
{
	Init();
}

GUIScore::~GUIScore()
{
	Clear();
}

void GUIScore::DrawBigScoreNumbers(unsigned int score, const SpriteRenderer *renderer) const
{
	std::string scoreString = std::to_string(score);
	int totalLength = 0;
	for (int i = 0; i < scoreString.size(); i++)
	{
		totalLength += mBigNumbers.find(scoreString[i])->second->GetWidth();
	}

	float posX = mScreenWidth * mFactorScreenWidthBig - totalLength * mFactorScreenWidthBig * mScreenScaling;
	float posY = mScreenHeight - mFactorScreenHeightBig * mScreenHeight;
	for (int i = 0; i < scoreString.size(); i++)
	{
		Sprite *sprite = mBigNumbers.find(scoreString[i])->second;
		int offset = sprite->GetWidth();
		renderer->DrawSprite(sprite, glm::vec2(posX, posY), 0.0f);
		posX += offset * mScreenScaling;
	}
}

bool GUIScore::DrawScoreBoard(unsigned int score, const SpriteRenderer *renderer, float dt)
{
	bool ret = false;
	mSBPosY += mAscendSpeed * mScreenScaling * dt;
	if (mSBPosY > mSBPosYFinal)
	{
		ret = true;
		mSBPosY = mSBPosYFinal;
	}

	renderer->DrawSprite(mScoreBoard, glm::vec2(mSBPosXFinal, mSBPosY), 0.0f);

	DrawSmallScoreNumbers(mScoreBoard, glm::vec2(mSBPosXFinal, mSBPosY), score, renderer, false);
	DrawSmallScoreNumbers(mScoreBoard, glm::vec2(mSBPosXFinal, mSBPosY), mBestScore, renderer, true);

	return ret;
}

void GUIScore::SaveMaxScore(unsigned int maxScore)
{
	mBestScore = maxScore;
	std::string dataPath = ResourceManager::GetPropString("GUIScore.DataPath");
	std::ofstream ofs(dataPath, std::ios::trunc);
	if (!ofs.is_open())
	{
		char buffer[100];
		strerror_s(buffer, 100, errno);
		std::cout << "Error opening " << dataPath << ":" << buffer << std::endl;
		return;
	}
	ofs << "BestScore:" << mBestScore << std::endl;
}

unsigned int GUIScore::GetBestScore() const
{
	return mBestScore;
}

void GUIScore::ResetSBHeightPos()
{
	mSBPosY = mSBInitialPosY;
}

void GUIScore::Init()
{
	LoadBestScore();

	std::string textureName = ResourceManager::GetPropString("GUIScore.FlappyBirdSpriteAtlasName");
	const Texture2D &texture = ResourceManager::GetTexture(textureName);

	mFactorScreenWidthBig = ResourceManager::GetPropFloat("GUIScore.FactorScreenWidthBig");
	mFactorScreenHeightBig = ResourceManager::GetPropFloat("GUIScore.FactorScreenHeightBig");
	
	//width and height of the sprites 0, 2 to 9 for big size sprites
	int widthBig = ResourceManager::GetPropInt("GUIScore.WidthBig");
	int heightBig = ResourceManager::GetPropInt("GUIScore.HeightBig");

	//all big number sprites share the Y position
	int originYBig = ResourceManager::GetPropInt("GUIScore.OriginYBig"); 
	
	//Create big size number 1 sprite - number 1 has its own width
	int num1WidthBig = ResourceManager::GetPropInt("GUIScore.Num1WidthBig");
	int num1XBig = ResourceManager::GetPropInt("GUIScore.Num1XBig");
	Sprite *sprite = new Sprite(texture, num1XBig, originYBig, num1WidthBig, heightBig);
	mBigNumbers.insert(std::make_pair('1', sprite));

	mSBEdgeOffsetWidth = ResourceManager::GetPropFloat("GUIScore.SBEdgeOffsetWidth");
	mSBEdgeOffsetHeight = ResourceManager::GetPropFloat("GUIScore.SBEdgeOffsetHeight");

	mBestEdgeOffsetWidth = ResourceManager::GetPropFloat("GUIScore.BestEdgeOffsetWidth");
	mBestEdgeOffsetHeight = ResourceManager::GetPropFloat("GUIScore.BestEdgeOffsetHeight");

	//width and height of the sprites 0, 2 to 9 for small size sprites
	int widthSmall = ResourceManager::GetPropInt("GUIScore.WidthSmall");
	int heightSmall = ResourceManager::GetPropInt("GUIScore.HeightSmall");
	
	//all small number sprites share the X position
	int originXSmall = ResourceManager::GetPropInt("GUIScore.OriginXSmall");

	//Create small size number 1 sprite - number 1 has its own width
	int num1WidthSmall = ResourceManager::GetPropInt("GUIScore.Num1WidthSmall");
	int num1Ysmall = ResourceManager::GetPropInt("GUIScore.Num1YSmall");
	sprite = new Sprite(texture, originXSmall, num1Ysmall, num1WidthSmall, heightSmall);
	mSmallNumbers.insert(std::make_pair('1', sprite));
	
	for (int i = 0; i < 10; i++)
	{
		if (i == 1)
			continue;

		std::string prop = "GUIScore.Num" + std::to_string(i) + "XBig";
		sprite = new Sprite(texture, ResourceManager::GetPropInt(prop), originYBig, widthBig, heightBig);
		mBigNumbers.insert(std::make_pair(std::to_string(i)[0], sprite));

		prop = "GUIScore.Num" + std::to_string(i) + "YSmall";
		sprite = new Sprite(texture, originXSmall, ResourceManager::GetPropInt(prop), widthSmall, heightSmall);
		mSmallNumbers.insert(std::make_pair(std::to_string(i)[0], sprite));
	}

	//load multiplicative factors for the score board
	mSBFactorScreenWidth = ResourceManager::GetPropFloat("GUIScore.SBFactorScreenWidth");
	mSBFactorScreenHeight = ResourceManager::GetPropFloat("GUIScore.SBFactorScreenHeight");

	//Create score board sprite
	int scoreBoardOriginX = ResourceManager::GetPropInt("GUIScore.ScoreBoardX");
	int scoreBoardOriginY = ResourceManager::GetPropInt("GUIScore.ScoreBoardY");
	int scoreBoardWidth = ResourceManager::GetPropInt("GUIScore.ScoreBoardWidth");
	int scoreBoardHeight = ResourceManager::GetPropInt("GUIScore.ScoreBoardHeight");
	mScoreBoard = new Sprite(texture, scoreBoardOriginX, scoreBoardOriginY, scoreBoardWidth, scoreBoardHeight);

	mSBPosXFinal = mScreenWidth * mSBFactorScreenWidth - mScoreBoard->GetWidth() * mSBFactorScreenWidth * mScreenScaling;
	mSBPosYFinal = mScreenHeight - mSBFactorScreenHeight * mScreenHeight;
	mSBInitialPosY = -scoreBoardHeight * mScreenScaling;
	ResetSBHeightPos();
	mAscendSpeed = ResourceManager::GetPropFloat("GUIScore.AscendSpeed");
}

void GUIScore::DrawSmallScoreNumbers(const Sprite *scoreBoard, const glm::vec2 &scoreBoardPos, int score, const SpriteRenderer *renderer, bool isBestScore) const
{
	std::string scoreString = std::to_string(score);
	int totalLength = 0;
	for (int i = 0; i < scoreString.size(); i++)
	{
		totalLength += mSmallNumbers.find(scoreString[i])->second->GetWidth();
	}

	float posX;
	float posY;
	if (!isBestScore)
	{
		posX = scoreBoardPos.x + scoreBoard->GetWidth() * mScreenScaling - mSBEdgeOffsetWidth * mScreenScaling - totalLength * mScreenScaling;
		posY = scoreBoardPos.y + scoreBoard->GetHeight() * mScreenScaling - mSBEdgeOffsetHeight * mScreenScaling;
	}
	else
	{
		posX = scoreBoardPos.x + scoreBoard->GetWidth() * mScreenScaling - mBestEdgeOffsetWidth * mScreenScaling - totalLength * mScreenScaling;
		posY = scoreBoardPos.y + scoreBoard->GetHeight() * mScreenScaling - mBestEdgeOffsetHeight * mScreenScaling;
	}

	for (int i = 0; i < scoreString.size(); i++)
	{
		Sprite *sprite = mSmallNumbers.find(scoreString[i])->second;
		int offset = sprite->GetWidth();
		renderer->DrawSprite(sprite, glm::vec2(posX, posY), 0.0f);
		posX += offset * mScreenScaling;
	}
}

void GUIScore::LoadBestScore()
{
	std::string dataPath = ResourceManager::GetPropString("GUIScore.DataPath");
	std::ifstream ifs(dataPath);
	if (!ifs.is_open())
	{
		char buffer[100];
		strerror_s(buffer, 100, errno);
		std::cout << "Error opening " << dataPath << ":" << buffer << std::endl;
		return;
	}
	std::string bestScore;
	std::string dumb;
	std::getline(ifs, dumb, ':');
	std::getline(ifs, bestScore);
	mBestScore = atoi(bestScore.c_str());
	ifs.close();
}

void GUIScore::Clear()
{
	for (auto pair : mBigNumbers)
	{
		DELETE_PTR(pair.second);
	}
	mBigNumbers.clear();

	for (auto pair : mSmallNumbers)
	{
		DELETE_PTR(pair.second);
	}
	mSmallNumbers.clear();

	DELETE_PTR(mScoreBoard);
}

