#include "crenderutils.h"

//TriangleTest\main.cpp
#include "window.h"
#include "vertex.h"
int main()
{

	Window window;
	window.init();

	Vertex vert[3] = {  {   0, .5f,0,1},
						{ .5f,-.5f,0,1},
						{-.5f,-.5f,0,1}};
	unsigned tris[3] = { 0,1,2 };

	const char vsource[] =
		"#version 150\n"
		"in vec4 position;"
		"void main() {gl_Position = position; }";

	const char fsource[] =
		"#version 150\n"
		"out vec4 outColor;"
		"void main() {outColor = vec4(1.0,0.0,0.0,1.0); }";

	Geometry geo = makeGeometry(vert, 3, tris, 3);
	Shader shady = makeShader(vsource, fsource);

	while (window.step())
	{
		draw(shady, geo);
	}

	freeGeometry(geo);
	freeShader(shady);
	window.term();

	return 0;
}