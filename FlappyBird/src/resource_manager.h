#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "shader.h"
#include "texture2D.h"
#include <string>
#include <glad/glad.h>
#include <map>

class ResourceManager final
{
public:
	static void LoadShader(const std::string &shaderName, const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
	static void LoadTexture(const std::string &textureName, const std::string &texturePath);
	static void LoadProperties(const std::string &propertiesFilePath);
	static const Shader& GetShader(const std::string &shaderName);
	static const Texture2D& GetTexture(const std::string &textureName);
	static const std::string& GetPropString(const std::string &propKeyString);
	static int GetPropInt(const std::string &propKeyInt);
	static float GetPropFloat(const std::string &propKeyFloat);

	static void Clear();
	
private:
	ResourceManager();
	static std::map<std::string, Shader> mShaders;
	static std::map<std::string, Texture2D> mTextures;
	static std::map<std::string, std::string> mProperties;
};
#endif
