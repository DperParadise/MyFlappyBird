#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
{
	std::string vertexShader = ReadShaderFromFile(vertexShaderFile);
	std::string fragmentShader = ReadShaderFromFile(fragmentShaderFile);

	Compile(vertexShader.c_str(), fragmentShader.c_str());
}

Shader::~Shader() {}

void Shader::Use() const
{
	glUseProgram(this->id);
}

void Shader::SetFloat(const char *name, float value) const
{
	glUniform1f(glGetUniformLocation(this->id, name), value);
}

void Shader::SetInteger(const char *name, int value) const
{
	glUniform1i(glGetUniformLocation(this->id, name), value);
}

void Shader::SetVector2f(const char *name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(this->id, name), x, y);
}

void Shader::SetVector2fv(const char *name, const glm::vec2 &vec2) const
{
	glUniform2f(glGetUniformLocation(this->id, name), vec2.x, vec2.y);
}

void Shader::SetVector3f(const char *name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}

void Shader::SetVector3fv(const char *name, const glm::vec3 &vec3) const
{
	glUniform3f(glGetUniformLocation(this->id, name), vec3.x, vec3.y, vec3.z);
}

void Shader::SetVector4f(const char *name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}

void Shader::SetVector4fv(const char *name, const glm::vec4 &vec4) const
{
	glUniform4f(glGetUniformLocation(this->id, name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::SetMatrix4(const char *name, const glm::mat4 &mat4) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, glm::value_ptr(mat4));
}

std::string Shader::ReadShaderFromFile(const std::string &filePath) const
{
	std::string shader = "error";
	std::ifstream ifs(filePath);
	if (!ifs.is_open())
	{
		std::cout << "Error opening shader file" << filePath << std::endl;
		return shader;
	}
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	shader = buffer.str();
	buffer.clear();
	ifs.close();

	return shader;
}

void Shader::Compile(const char *vertexShaderCode, const char * fragmentShaderCode)
{
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vShader);
	CheckCompLinkErrors(vShader, "SHADER");

	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fShader);
	CheckCompLinkErrors(fShader, "SHADER");

	this->id = glCreateProgram();
	glAttachShader(this->id, vShader);
	glAttachShader(this->id, fShader);
	glLinkProgram(this->id);
	CheckCompLinkErrors(this->id, "PROGRAM");
}

void Shader::CheckCompLinkErrors(GLuint object, std::string type) const
{
	GLint success;
	const int logLength = 1024;
	GLchar log[logLength];
	if (type == "SHADER")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, logLength, NULL, log);
			std::cout << "Error in shader:  " << log << std::endl;
			memset(log, 0, logLength);
		}
	}
	else //type will be PROGRAM
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, logLength, NULL, log);
			std::cout << "Error linking program: " << log << std::endl;
			memset(log, 0, logLength);
		}
	}
}


