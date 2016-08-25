#pragma once


class Window
{
private:
	int width, height;
	char title[64];
	bool isInitalized = false;

	//forward declare pointers
	class GLFWwindow *windHandle = nullptr;

public:
	bool init(int a_width = 800, int a_height = 600, char *a_title = "Test is best");	//start
	bool step();																		//update
	bool term();																		//termination
};