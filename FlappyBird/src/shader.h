#ifndef __SHADER_H__
#define __SHADER_H__

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>

 class Shader final
{
public:
	Shader(const std::string &vertexShader, const std::string &fragmentShader);
	~Shader();

	void Use() const;

	//Set float as a uniform value to the shader
	void SetFloat(const char *name, float value) const;

	//Set integer as a uniform value to the shader
	void SetInteger(const char *name, int value) const;

	//Set 2 float vector components as a uniform values to the shader
	void SetVector2f(const char *name, float x, float y) const;

	//Set a 2 dimension vector as a uniform value to the shader
	void SetVector2fv(const char *name, const glm::vec2 &vec2) const;

	//Set 3 float vector components as a uniform values to the shader
	void SetVector3f(const char *name, float x, float y, float z) const;

	//Set a 3 dimension vector as a uniform value to the shader
	void SetVector3fv(const char *name, const glm::vec3 &vec3) const;

	//Set 4 float vector components as a uniform values to the shader
	void SetVector4f(const char *name, float x, float y, float z, float w) const;

	//Set a 4 dimension vector as a uniform value to the shader
	void SetVector4fv(const char *name, const glm::vec4 &vec4) const;

	//Set a 4x4 matrix of floats as a uniform value to the shader
	void SetMatrix4(const char *name, const glm::mat4 &mat4) const;

private:
	int id;
	
	std::string ReadShaderFromFile(const std::string &filePath) const;
	void Compile(const char *vertexShaderCode, const char * fragmentShaderCode);

	/* Check whether the compilation/linkage was successful or not.
	 @param object: shader/program id
	 @param type: object of type SHADER or PROGRAM
	*/
	void CheckCompLinkErrors(GLuint object, const std::string &type) const;

};


#endif
