#include "Resource.h"
#ifdef USEGLFW
#include <GL/glfw.h>
#include <stdlib.h>
#include "GLFW_OS.h"
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"GLFW.lib")
#pragma comment (lib,"glu32.lib")

GLFW_OS::GLFW_OS(void)
{
}


GLFW_OS::~GLFW_OS(void)
{
}

void GLFW_OS::Init()
{
	const int window_width = 800, window_height = 600;
	if(glfwInit()!=GL_TRUE)
		Safe_Exit(EXIT_FAILURE);

	//800x600 16bit color, no depth, alpha or stencil buffers. windowed
	if(glfwOpenWindow(window_width, window_height, 5,6,5,0,0,0,GLFW_WINDOW) != GL_TRUE)
		Safe_Exit(EXIT_FAILURE);
	glfwSetWindowTitle("The GLFW Window");

	glEnable(GL_DEPTH_TEST);
}

void GLFW_OS::PopUpWindow()
{

}

void GLFW_OS::Safe_Exit(int return_code)
{
	glfwTerminate();
	exit(return_code);
}
#endif // USEGLFW