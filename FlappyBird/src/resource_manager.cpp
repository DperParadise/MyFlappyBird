#include "resource_manager.h"
#include <iostream>
#include <glad/glad.h>

//Instantiate static variables
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture2D> ResourceManager::textures;

ResourceManager::ResourceManager() {}

void ResourceManager::LoadShader(const std::string &shaderName, const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
	if (shaders.find(shaderName) != shaders.end())
	{
		std::cout << "Error: shader with name:" << shaderName << " already in the resources map. Not loaded." << std::endl;
		return;
	}
	Shader shader = Shader(vertexShaderPath, fragmentShaderPath);
	shaders.insert(std::map<std::string, Shader>::value_type(shaderName, shader));
}

void ResourceManager::LoadTexture(const std::string &textureName, const std::string &texturePath)
{
	if (textures.find(textureName) != textures.end())
	{
		std::cout << "Error: texture with name: " << textureName << " already in the resources map. Not loaded" << std::endl;
		return;
	}
	Texture2D texture = Texture2D(texturePath.c_str());
	textures.insert(std::map<std::string, Texture2D>::value_type(textureName, texture));
}

const Shader& ResourceManager::GetShader(const std::string &shaderName)
{
	std::map<std::string, Shader>::iterator it = shaders.find(shaderName);
	if (it == shaders.end())
	{
		std::cout << "Error: no shader in the resources with the name:" << shaderName << std::endl;
	}
	return (*it).second;
}

const Texture2D& ResourceManager::GetTexture(const std::string &textureName)
{
	std::map<std::string, Texture2D>::iterator it = textures.find(textureName);
	if (it == textures.end())
	{
		std::cout << "Error: no texture in the resources with the name:" << textureName << std::endl;
	}
	return (*it).second;
}

void ResourceManager::Clear()
{
	for (auto iter : shaders)
	{
		iter.second.Clear();
	}

	for (auto iter : textures)
	{
		iter.second.Clear();
	}
}

