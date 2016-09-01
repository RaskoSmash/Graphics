#include "GLFW\glfw3.h"
#include "timer.h"

bool Timer::init()
{
	glfwSetTime(0);
	return true;
}

bool Timer::step()
{
	previousTime = currentTime;
	currentTime = getRealTime();
	deltaTime = currentTime - previousTime;

	return true;
}

bool Timer::term()
{
	return true;
}

float Timer::getDeltaTime() const
{
	return deltaTime;
}

float Timer::getTotalTime() const
{
	return currentTime;
}

float Timer::getRealTime() const
{
	return glfwGetTime();
}
