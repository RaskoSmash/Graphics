#pragma once
class Input
{
public:
	enum KEY_STATE { UP, DOWN, PRESS, RELEASE };
private:
	class GLFWwindow *w;
	KEY_STATE keys[350];
	double mouseX, mouseY;
	float mouseHorz, mouseVert;
public:
	
	bool init(const class Window &w);
	bool step();
	bool term();
	KEY_STATE getKeyState(int key) const
	{
		return keys[key];
	}

	float getMouseAxisHorizontal() const
	{
		return mouseHorz;
	}
	float getMouseAxisVertical() const
	{
		return mouseVert;
	}
};

