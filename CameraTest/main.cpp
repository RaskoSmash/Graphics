#include "crenderutils.h"

//TriangleTest\main.cpp
#include "window.h"
#include "vertex.h"
#include "Gallery.h"

#include "GLM\glm.hpp"
#include "GLM\ext.hpp"
#include "timer.h"
#include "input.h"
#include "camera.h"
int main()
{

	Window  window;
	Gallery gallery;
	Timer	time;
	Input	input;
	FlyCamera	flycam;

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
	model2 =  glm::rotate(180.f, glm::vec3(0, -1, 0));
	float dt = 0;
	float ct = 0;

	model3 =  glm::rotate(180.f, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(5, 5, 5));

	flycam.jumpTo(glm::vec3(20, 0, 0));
	flycam.lookAt(glm::vec3(0,0,0));

	while (window.step())
	{
		input.step();
		time.step();

		if(input.getKeyState('D') == Input::DOWN)
			ct += time.getDeltaTime();
		if (input.getKeyState('A') == Input::DOWN)
			ct -= time.getDeltaTime();
		

		view = flycam.getView();
		proj = flycam.getProjection();

		flycam.update(input, time, 5);

		model = glm::rotate(dt, glm::vec3(0, 1, 0));

		draw(gallery.getShader("CAMERA"), gallery.getObject("sphere"),
			glm::value_ptr(model2),
			glm::value_ptr(view),
			glm::value_ptr(proj), dt);

		draw(gallery.getShader("CAMERA"), gallery.getObject("cube"),
			glm::value_ptr(model),
			glm::value_ptr(view),
			glm::value_ptr(proj), dt, input);

		draw(gallery.getShader("CAMERA"), gallery.getObject("quad"),
			glm::value_ptr(model3),
			glm::value_ptr(view),
			glm::value_ptr(proj), dt);
	}

	input.term();
	time.term();
	gallery.term();
	window.term();
	return 0;
}
