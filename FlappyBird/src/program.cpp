#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "game.h"

//GLFW function declarations
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

//The screen scale factor
constexpr float screenScaling = 3.0f;

//The width of the screen
constexpr int SCREEN_WIDTH = (int)screenScaling * 144;

//The height of the screen
constexpr int SCREEN_HEIGHT = (int)screenScaling * 256;

Game FlappyBirdGame(SCREEN_WIDTH, SCREEN_HEIGHT, screenScaling);

int main()
{
	//GLFW configuration
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MyFlappyBird", NULL, NULL);
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	
	
	float dt = 0.0f;
	float currentTime = 0.0f;
	float previousTime = 0.0f;
	
	FlappyBirdGame.Init();

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		currentTime = glfwGetTime();
		dt = currentTime - previousTime;
		previousTime = currentTime;

		FlappyBirdGame.ProcessInput();
		FlappyBirdGame.Update(dt);
		FlappyBirdGame.Render(dt);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	//If the user press ESC key, we close the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		FlappyBirdGame.mKeys[GLFW_KEY_SPACE] = true;
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		FlappyBirdGame.mKeys[GLFW_KEY_SPACE] = false;
		FlappyBirdGame.mKeysProcessed[GLFW_KEY_SPACE] = false;
	}
		
}


