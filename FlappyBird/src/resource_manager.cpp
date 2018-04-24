#include "resource_manager.h"
#include <iostream>
#include <glad/glad.h>
#include <fstream>

//Instantiate static variables
std::map<std::string, Shader> ResourceManager::mShaders;
std::map<std::string, Texture2D> ResourceManager::mTextures;
std::map<std::string, std::string> ResourceManager::mProperties;

ResourceManager::ResourceManager() {}

void ResourceManager::LoadShader(const std::string &shaderName, const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
	if (mShaders.find(shaderName) != mShaders.end())
	{
		std::cout << "Error: shader with name:" << shaderName << " already in the resources map. Not loaded." << std::endl;
		return;
	}
	Shader shader = Shader(vertexShaderPath, fragmentShaderPath);
	mShaders.insert(std::map<std::string, Shader>::value_type(shaderName, shader));
}

void ResourceManager::LoadTexture(const std::string &textureName, const std::string &texturePath)
{
	if (mTextures.find(textureName) != mTextures.end())
	{
		std::cout << "Error: texture with name: " << textureName << " already in the resources map. Not loaded" << std::endl;
		return;
	}
	Texture2D texture = Texture2D(texturePath.c_str());
	mTextures.insert(std::map<std::string, Texture2D>::value_type(textureName, texture));
}

void ResourceManager::LoadProperties(const std::string &propertiesFilePath)
{
	std::ifstream inputFileStream(propertiesFilePath);
	if (!inputFileStream.is_open())
	{
		char buffer[100];
		strerror_s(buffer, 100, errno);
		std::cout << "Error opening properties file: " << buffer << std::endl;
		return;
	}
	
	std::string key;
	std::string value;
	while (inputFileStream)
	{
		std::getline(inputFileStream, key, ':');
		std::getline(inputFileStream, value);
		mProperties.insert(std::map<std::string, std::string>::value_type(key, value));
	}
}

const Shader& ResourceManager::GetShader(const std::string &shaderName)
{
	std::map<std::string, Shader>::iterator it = mShaders.find(shaderName);
	if (it == mShaders.end())
	{
		std::cout << "Error: no shader in the resources with the name:" << shaderName << std::endl;
	}
	return (*it).second;
}

const Texture2D& ResourceManager::GetTexture(const std::string &textureName)
{
	std::map<std::string, Texture2D>::iterator it = mTextures.find(textureName);
	if (it == mTextures.end())
	{
		std::cout << "Error: no texture in the resources with the name:" << textureName << std::endl;
	}
	return (*it).second;
}

const std::string& ResourceManager::GetPropString(const std::string &propKeyString)
{
	std::map<std::string, std::string>::iterator it = mProperties.find(propKeyString);
	if (it == mProperties.end())
	{
		std::cout << "Error: no property found with the name: " << propKeyString << std::endl;
		return propKeyString; 
	}
	return (*it).second;
}

int ResourceManager::GetPropInt(const std::string &propKeyInt)
{
	std::map<std::string, std::string>::iterator it = mProperties.find(propKeyInt);
	if (it == mProperties.end())
	{
		std::cout << "Error: no property found with the name:" << propKeyInt << std::endl;
		return -1;
	}
	return std::stoi((*it).second);
}

float ResourceManager::GetPropFloat(const std::string &propKeyFloat)
{
	std::map<std::string, std::string>::iterator it = mProperties.find(propKeyFloat);
	if (it == mProperties.end())
	{
		std::cout << "Error: no property found with the name:" << propKeyFloat << std::endl;
		return -1.0f;
	}
	return std::stof((*it).second);
}

void ResourceManager::Clear()
{
	for (auto iter : mShaders)
	{
		iter.second.Clear();
	}

	for (auto iter : mTextures)
	{
		iter.second.Clear();
	}
}

