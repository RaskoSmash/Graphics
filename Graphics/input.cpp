#include "GLFW\glfw3.h"
#include "window.h"
#include "input.h"
#include <cstdio>
#include <cstring>

bool Input::init(const Window &win)
{
	if (!win.isInitalized) return false;
	memset(keys, 0, 350 * sizeof(KEY_STATE));
	w = win.windHandle;

	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	mouseX = mouseY = mouseHorz = mouseVert = 0;

	return true;
}

bool Input::step()
{
	for(int i = 0; i < 350; ++i)
	{
		//GLFW_PRESS, GLFW_RELEASE
		int res = glfwGetKey(w, i);

		if ((keys[i] == UP || keys[i] == RELEASE)
											&& res == GLFW_PRESS)
		{
			keys[i] = PRESS;
		}
		else if ((keys[i] == DOWN || keys[i] == PRESS) 
											&& res == GLFW_RELEASE)
		{
			keys[i] = RELEASE;
		}
		else if (keys[i] == PRESS)
		{
			keys[i] = DOWN;
		}
		else if (keys[i] == RELEASE)
		{
			keys[i] = UP;
		}
	}

	double prevX = mouseX;
	double prevY = mouseY;
	glfwGetCursorPos(w, &mouseX, &mouseY);

	mouseHorz = mouseX - prevX;
	mouseVert = mouseY - prevY;

	printf("%f %f\n", mouseX, mouseY);

	//	int h, w;
//	glfwGetWindowSize();

	return true;
}

bool Input::term()
{
	return true;
}
