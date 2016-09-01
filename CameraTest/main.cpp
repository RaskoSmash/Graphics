#include "crenderutils.h"

//TriangleTest\main.cpp
#include "window.h"
#include "vertex.h"
#include "Gallery.h"

#include "GLM\glm.hpp"
#include "GLM\ext.hpp"
int main()
{

	Gallery gallery;
	Window window;
	window.init(600, 600);

	//Vertex vert[3] = { { 0,  .5f,0,1, 1,0,0,1 },
	//{ .5f, -.5f,0,1, 0,1,0,1 },
	//{ -.5f, -.5f,0,1, 0,1,1,1 } };
	//unsigned tris[3] = { 0,1,2 };
	//Vertex vats[3] = { { 1,  .5f,0,1, 0,1,1,1 },
	//{ .5f, -.5f,0,1, 0,1,0,1 },
	//{ -.5f, -.5f,0,1, 1,0,0,1 } };
	//unsigned trees[3] = { 1,2,0 };
	//const char vsource[] =
	//	"#version 330\n"
	//	"layout(location = 0)in vec4 position;"
	//	"layout(location = 1)in vec4 color;"
	//	"out vec4 vColor;"
	//	"void main() { vColor = color; gl_Position = position; } ";
	//const char fsource[] =
	//	"#version 150\n"
	//	"in vec4 vColor;"
	//	"out vec4 outColor;"
	//	"void main () { outColor = vec4(1,0,0,1) - vColor; } ";
	//Geometry geo3 = makeGeometry(vert, 3, tris, 3);
	//Shader shady3 = makeShader(vsource, fsource);
	//Geometry geo1 = loadObj("../res/models/sphere.obj");
	//Shader shady1 = loadShader("../res/shaders/simpleVert.txt","../res/shaders/simpleFrag.txt");
	////Geometry geo2 = makeGeometry(vats, 3, trees, 3);
	////Shader shady2 = makeShader(vsource, fsource);

	float IDENTITY[16] = {1,0,0,0,	//right
						  0,1,0,0,	//up	
						  0,0,1,0,	//forward
						  0,0,0,1};	//left

	glm::mat4 proj, view, model1, model2;

	proj = glm::ortho<float>(-10,10,-10,10,-10,10);

	//proj = glm::perspective(45.0f, 1.f, .1f, 10.f);

	

	model1 = glm::scale(glm::vec3(3.0f, 3.f, 3.f)) * glm::translate(glm::vec3(0.5f, 0.0f, 0.0f));
	model2 = glm::scale(glm::vec3(0.5f, .5f, .5f)) * glm::translate(glm::vec3(0.0f, 2.0f, 0.0f));

	gallery.init();

	gallery.loadShader("CAMERA", "../res/shaders/cameraVert.txt", "../res/shaders/cameraFrag.txt");
	gallery.loadObjectOBJ("CUBE", "../res/models/cube.obj");

	float time = 0;

	while (window.step())
	{
		//time is a uniform
		time += 0.01666667;

		view = glm::lookAt(glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.f, 0.f, 0.f));


		model2 = glm::rotate(time, glm::vec3(0, 1, 0));

		draw(gallery.getShader("CAMERA"), gallery.getObject("CUBE"), glm::value_ptr(proj), 
																	   glm::value_ptr(view), 
																	   glm::value_ptr(model1 * glm::translate(glm::vec3(1,1,1))));
		draw(gallery.getShader("CAMERA"), gallery.getObject("CUBE"), glm::value_ptr(proj),
			glm::value_ptr(view),
			glm::value_ptr(model2 * glm::translate(glm::vec3(-3, 1, 1))));
		//draw(shady2, geo2);
		//draw(shady2, geo2);
	}

	/*freeGeometry(geo1);
	freeShader(shady1);*/
	gallery.term();
	window.term();

	return 0;
}
