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
#include "animation.h"
#include "game_object.h"
#include "game_level.h"
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	
	//beginTEST
	glm::mat4 projection;
	projection = glm::ortho(0.0f, float(SCREEN_WIDTH), 0.0f, float(SCREEN_HEIGHT), -1.0f, 1.0f);
	ResourceManager::LoadShader("testShader", "resources/shaders/test.vs", "resources/shaders/test.fs");
	ResourceManager::GetShader("testShader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("testShader").SetInteger("textureSampler", 0);
	ResourceManager::LoadTexture("flappyBirdAtlas", "resources/textures/flappy_bird_sprite_sheet.png");
	ResourceManager::LoadTexture("awesomeface", "resources/textures/awesomeface.png");

	SpriteRenderer *renderer = new SpriteRenderer(ResourceManager::GetShader("testShader"), 3.0f);

	GameLevel *gameLevel = new GameLevel(SCREEN_WIDTH, SCREEN_HEIGHT, 3.0f, 64.0f, 64.0f, -150.0f, 150.0f, 100.f, renderer);

	std::vector<Sprite*> flySprites;
	flySprites.push_back(new Sprite(ResourceManager::GetTexture("flappyBirdAtlas"), 3, 491, 17, 12));
	flySprites.push_back(new Sprite(ResourceManager::GetTexture("flappyBirdAtlas"), 31, 491, 17, 12));
	flySprites.push_back(new Sprite(ResourceManager::GetTexture("flappyBirdAtlas"), 59, 491, 17, 12));
	Animation *flyAnimation = new Animation(flySprites,8.0);
	GameObject *pollo = new GameObject(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(0.0f, 200.0f), -80.0f, flyAnimation);

	float dt = 0.0f;
	float currentTime = 0.0f;
	float previousTime = 0.0f;
	//endTEST

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//beginTEST

		currentTime = glfwGetTime();
		dt = currentTime - previousTime;
		previousTime = currentTime;

		gameLevel->UpdateColumnsPosition(dt);
		pollo->UpdatePosition(dt);

		gameLevel->DrawLevel(dt);
		pollo->Draw(renderer, dt);
	
		//endTEST

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();

	//beginTEST
	delete renderer;

	delete flyAnimation;
	delete pollo;

	delete gameLevel;

	//endTEST

	return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	//If the user press ESC key, we close the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);


}


