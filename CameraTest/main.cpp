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
#include "procgen.h"

int main()
{

	Window  window;
	Gallery gallery;
	Timer	timer;
	Input	input;

	window.init(1280, 720);

	glm::mat4 view = glm::lookAt(glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 proj = glm::perspective(45.f, 16/9.f, 1.f, 100.f);
	glm::mat4 modelC;
	glm::mat4 modelS;

	Geometry cube = loadObj("../res/models/cube.obj");
	Shader shady = loadShader("../res/shaders/phongVert.txt", "../res/shaders/phongFrag.txt");
	Geometry sphere = loadObj("../res/models/sphere.obj");
	Geometry bunny = loadObj("../res/models/soulspear.obj");

	float time = 0;
	while (window.step())
	{
		modelS = glm::translate(glm::vec3(0, cos(time), 0));
		time += 0.016666667;
		modelC = glm::rotate(time, glm::vec3(0, 1, 1));

		drawPhong(shady, bunny, glm::value_ptr(modelC),
								glm::value_ptr(view), 
							glm::value_ptr(proj));

		drawPhong(shady, sphere, glm::value_ptr(modelS),
			glm::value_ptr(view),
			glm::value_ptr(proj));
	}


	window.term();
	return 0;
}
