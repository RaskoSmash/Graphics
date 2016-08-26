#include "crenderutils.h"

//TriangleTest\main.cpp
#include "window.h"
#include "vertex.h"
int main()
{

	Window window;
	window.init();

	Vertex vert[3] = { { 0,  .5f,0,1, 1,0,0,1 },
					   { .5f, -.5f,0,1, 0,1,0,1 },
					   { -.5f, -.5f,0,1, 0,1,1,1 } };
	unsigned tris[3] = { 0,1,2 };

	Vertex vats[3] = { { 1,  .5f,0,1, 0,1,1,1 },
					   { .5f, -.5f,0,1, 0,1,0,1 },
					   { -.5f, -.5f,0,1, 1,0,0,1 } };

	unsigned trees[3] = { 1,2,0 };

	const char vsource[] =
		"#version 330\n"
		"layout(location = 0)in vec4 position;"
		"layout(location = 1)in vec4 color;"
		"out vec4 vColor;"
		"void main() { vColor = color; gl_Position = position; } ";

	const char fsource[] =
		"#version 150\n"
		"in vec4 vColor;"
		"out vec4 outColor;"
		"void main () { outColor = vColor; } ";

	Geometry geo1 = makeGeometry(vert, 3, tris, 3);
	Shader shady1 = makeShader(vsource, fsource);

	Geometry geo2 = makeGeometry(vats, 3, trees, 3);
	Shader shady2 = makeShader(vsource, fsource);

	while (window.step())
	{
		draw(shady1, geo1);
		draw(shady2, geo2);
	}

	freeGeometry(geo1);
	freeShader(shady1);
	window.term();

	return 0;
}