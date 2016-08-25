#include "gldecs.h"
#include "window.h"
#include <cstring>

bool Window::init(int a_width, int a_height, char * a_title)
{
	width = a_width;
	height = a_height;
	strcpy_s(title, 64, a_title);

	glfwInit();
	windHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(windHandle);
	isInitalized = true;

	glewExperimental = true;
	glewInit();
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	return true;
}

bool Window::step()
{
	if (!isInitalized) return false;

	glfwPollEvents();
	glfwSwapBuffers(windHandle);
	glClear(GL_COLOR_BUFFER_BIT);

	return !glfwWindowShouldClose(windHandle);
}

bool Window::term()
{
	glfwDestroyWindow(windHandle);
	glfwTerminate();
	windHandle = nullptr;
	isInitalized = false;
	return true;
}
