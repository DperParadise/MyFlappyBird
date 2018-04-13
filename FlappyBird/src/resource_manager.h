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
	static const Shader& GetShader(const std::string &shaderName);
	static const Texture2D& GetTexture(const std::string &textureName);
	
private:
	ResourceManager();
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;
};
#endif
