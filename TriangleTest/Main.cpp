#include "crenderutils.h"

//TriangleTest\main.cpp
#include "window.h"

int main()
{
	Window window;
	window.init();

	while (window.step());

	window.term();

	return 0;
}