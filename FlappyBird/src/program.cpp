#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//beginTEST
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "texture2D.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
//endTEST

//GLFW function declarations
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

//The width of the screen
constexpr int SCREEN_WIDTH = 3 * 144;

//The height of the screen
constexpr int SCREEN_HEIGHT = 3 * 256;

int main()
{
	//GLFW configuration
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MyFloppyBird", NULL, NULL);
	glfwMakeContextCurrent(window);

	//Load OpenGL functions pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
	}

	//Set glfw callback functions
	glfwSetKeyCallback(window, KeyCallback);

	//OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//beginTEST
	
	glm::mat4 projection;
	projection = glm::ortho(0.0f, float(SCREEN_WIDTH), 0.0f, float(SCREEN_HEIGHT), -1.0f, 1.0f);
	ResourceManager::LoadShader("testShader", "resources/shaders/test.vs", "resources/shaders/test.fs");
	ResourceManager::GetShader("testShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("testShader").SetVector3f("color", 1.0f, 0.0f, 0.0f);
	ResourceManager::GetShader("testShader").SetInteger("textureSampler", 0);
	ResourceManager::LoadTexture("awesomeface", "resources/textures/awesomeface.png");

	SpriteRenderer *renderer = new SpriteRenderer(ResourceManager::GetShader("testShader"));
	
	//endTEST

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//beginTEST
		renderer->DrawSprite(ResourceManager::GetTexture("awesomeface"), glm::vec2(SCREEN_WIDTH * 0.5f - 50.0f, SCREEN_HEIGHT * 0.5f - 50.0f), 45.0f, glm::vec2(100.0f));
		//endTEST

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	delete renderer;

	return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	//If the user press ESC key, we close the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);


}


