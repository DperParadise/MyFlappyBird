#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();




		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	//If the user press ESC key, we close the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);


}


