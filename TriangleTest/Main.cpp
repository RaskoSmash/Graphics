

#include "crenderutils.h"
#include "window.h"
#include "Gallery.h"
#include "timer.h"
#include "input.h"
#include "vertex.h"
#include "camera.h"

#include "../dep/include/GLM/glm.hpp"
#include "../dep/include/GLM/ext.hpp"

int main()
{
	Window	window;
	Gallery gallery;
	Timer	time;
	Input   input;

	window.init(1280, 720);
	gallery.init();
	input.init(window);
	time.init();

	Vertex verts[] = { { 1,1,0,1 },{ 1,-1,0,1 },{ -1,-1,0,1 },{ -1,1,0,1 } };

	unsigned tris[] = { 0,1,2, 2,3,0 };


	gallery.loadShader("CAMERA", "../res/shaders/cameraVert.txt",
		"../res/shaders/cameraFrag.txt");

	gallery.makeObject("quad", verts, 4, tris, 6);
	gallery.loadObjectOBJ("sphere", "../res/models/sphere.obj");

	gallery.loadObjectOBJ("cube", "../res/models/cube.obj");

	float IDENTITY[16] = { 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };

	glm::mat4 proj, view, model, model2, model3;


	//proj = glm::ortho<float>(-10, 10, -10, 10, -10, 10);

	proj = glm::perspective(45.f, 1.f, 1.f, 50.f);

	//proj = glm::perspective(45.f, 1.f, .1f, 10.f);
	model2 = glm::translate(glm::vec3(1, 0, 1)) * glm::rotate(180.f, glm::vec3(0, -1, 0));
	float dt = 0;

	model3 = glm::translate(glm::vec3(0, 0, 0))  * glm::rotate(180.f, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(5, 5, 5));

	float ct = 0;

	FlyCamera cam;
	cam.jumpTo(glm::vec3(0, 0, -10));
	cam.lookAt(glm::vec3(0, 0, 0));


	while (window.step())
	{
		time.step();
		input.step();




		ct += time.getDeltaTime();
		view = cam.getView();
		proj = cam.getProjection();

		cam.update(input, time);

		model = glm::translate(glm::vec3(0, ct, 0)) *
			glm::rotate(ct, glm::vec3(0, 1, 0));

		draw(gallery.getShader("CAMERA"), gallery.getObject("sphere"),
			glm::value_ptr(model2),
			glm::value_ptr(view),
			glm::value_ptr(proj), ct);

		draw(gallery.getShader("CAMERA"), gallery.getObject("cube"),
			glm::value_ptr(model),
			glm::value_ptr(view),
			glm::value_ptr(proj), ct);

		draw(gallery.getShader("CAMERA"), gallery.getObject("quad"),
			glm::value_ptr(model3),
			glm::value_ptr(view),
			glm::value_ptr(proj), ct);
	}

	input.term();
	time.term();
	gallery.term();
	window.term();
	return 0;
}

//before using files

//#include "crenderutils.h"
//
////TriangleTest\main.cpp
//#include "window.h"
//#include "vertex.h"
//int main()
//{
//
//	Window window;
//	window.init();
//
//	Vertex vert[3] = { { 0,  .5f,0,1, 1,0,0,1 },
//					   { .5f, -.5f,0,1, 0,1,0,1 },
//					   { -.5f, -.5f,0,1, 0,1,1,1 } };
//	unsigned tris[3] = { 0,1,2 };
//
//	Vertex vats[3] = { { 1,  .5f,0,1, 0,1,1,1 },
//					   { .5f, -.5f,0,1, 0,1,0,1 },
//					   { -.5f, -.5f,0,1, 1,0,0,1 } };
//
//	unsigned trees[3] = { 1,2,0 };
//
//	const char vsource[] =
//		"#version 330\n"
//		"layout(location = 0)in vec4 position;"
//		"layout(location = 1)in vec4 color;"
//		"out vec4 vColor;"
//		"void main() { vColor = color; gl_Position = position; } ";
//
//	const char fsource[] =
//		"#version 150\n"
//		"in vec4 vColor;"
//		"out vec4 outColor;"
//		"void main () { outColor = vec4(1,0,0,1) - vColor; } ";
//
//	Geometry geo1 = makeGeometry(vert, 3, tris, 3);
//	Shader shady1 = makeShader(vsource, fsource);
//
//	Geometry geo2 = makeGeometry(vats, 3, trees, 3);
//	Shader shady2 = makeShader(vsource, fsource);
//
//	while (window.step())
//	{
//		draw(shady1, geo1);
//		draw(shady2, geo2);
//	}
//
//	freeGeometry(geo1);
//	freeShader(shady1);
//	window.term();
//
//	return 0;
//}

