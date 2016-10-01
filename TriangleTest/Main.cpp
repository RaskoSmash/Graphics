
#include "crenderutils.h"
#include "camera.h"
#include "GLM\ext.hpp"
#include "timer.h"
#include "input.h"
#include "generation.h"

void main()
{
	Window context;
	context.init(1280, 720);
	FlyCamera cam;
	Input input;
	input.init(context);
	Timer time;
	time.init();


	Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);
	Geometry spear = loadObj("../res/models/soulspear.obj");
	Geometry sphere = loadObj("../res/models/sphere.obj");

	Texture spear_normal = loadTexture("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = loadTexture("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = loadTexture("../res/textures/soulspear_specular.tga");

	Texture height = makeHeightMap(128,64);

	const unsigned char norm_pixels[4] = { 127, 127, 255, 255 };
	Texture vertex_normals = makeTexture(1, 1, 4, norm_pixels);

	const unsigned char white_pixels[4] = { 255, 255, 255, 255 };
	Texture white = makeTexture(1, 1, 4, white_pixels);

	Shader qdraw = loadShader("../res/shaders/quad.vert", "../res/shaders/quad.frag", false);
	Shader gpass = loadShader("../res/shaders/gpass.vert", "../res/shaders/gpass.frag");

	//////////////////////////////
	//// Using Light pass shader designed for shadows.
	//// Note that shadow pass can disable face-culling for some back-shadow improvements.
	Shader spass = loadShader("../res/shaders/spass.vert", "../res/shaders/spass.frag", true, false, false);
	Shader lpass = loadShader("../res/shaders/lspass.vert", "../res/shaders/lspass.frag", false, true);


	FrameBuffer screen = { 0, 1280, 720 };
	FrameBuffer aster = makeFrameBuffer(720,720,1);

	bool flTex[] = { false, true, false, true }; // colors don't need floats, but position/normal should use it.
	FrameBuffer gframe = makeFrameBuffer(1280, 720, 4, flTex);
	FrameBuffer lframe = makeFrameBuffer(1280, 720, 3);


	// Temporary shadow framebuffer. Will be cleared and reused by each light!
	// Its RESOLUTION WILL GREATLY EFFECT THE QUALITY. Try playing around with high/low res.
	FrameBuffer sframe = makeFrameBuffer(1024, 1024, 0);



	// Camera information
	glm::mat4 camView = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	cam.lookAt(glm::vec3(camView[0][3], camView[1][3], camView[2][3]));
	glm::mat4 camProj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	// Model Matrices
	glm::mat4 spearModel; // ROTATES in main
	glm::mat4 sphereModel = glm::translate(glm::vec3(0.3f, -1, -0.2f));
	glm::mat4 wallModel = glm::rotate(45.f, glm::vec3(0, -1, 0)) * 
		glm::translate(glm::vec3(0, 0, -6)) * glm::scale(glm::vec3(10, 10, 1));

	// Light Matrices and data
	// They can all use the same projection matrix...
	glm::mat4 lightProj = glm::ortho<float>(-10, 10, -10, 10, -10, 10);

	glm::mat4   redView = glm::lookAt(glm::normalize(-glm::vec3(1, -1, -1)), 
		glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4   redColor = glm::vec4(1, 0, 0, 1);

	glm::mat4 greenView = glm::lookAt(glm::normalize(-glm::vec3(1, 1, -1)), 
		glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::vec4 greenColor = glm::vec4(0, 1, 0, 1);

	float times = 0;

	while (context.step())
	{
		time.step();
		input.step();

		times += time.getDeltaTime();
		camView = cam.getView();
		camProj = cam.getProjection();

		cam.update(input, time);

		spearModel = glm::rotate(times, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0));
		sphereModel = glm::translate(glm::vec3(0, 0, cosf(times) * 5));
		sphereModel *= glm::rotate(times, glm::vec3(0, 1, 0));
		/////////////////////////////////////////////////////
		// Geometry Pass
		//
		clearFrameBuffer(gframe);
		tdraw(gpass, spear, gframe, spearModel, camView, camProj, spear_diffuse, spear_normal, spear_specular);
		tdraw(gpass, sphere, gframe, sphereModel, camView, camProj, white, height, white);
		tdraw(gpass, quad, gframe, wallModel, camView, camProj, white, vertex_normals, white);

		//tdraw(blur, quad, nframe, gframe.colors[1]);

		/////////////////////////////////////////////////////
		//// Light pass!
		clearFrameBuffer(lframe);

		//////////////////////////
		// RED LIGHT

		// Shadow PrePass
		clearFrameBuffer(sframe);
		tdraw(spass, spear, sframe, spearModel, redView, lightProj);
		tdraw(spass, sphere, sframe, sphereModel, redView, lightProj);
		tdraw(spass, quad, sframe, wallModel, redView, lightProj);
		// Light Aggregation
		tdraw(lpass, quad, lframe, camView,
			gframe.colors[0], gframe.colors[1], gframe.colors[2], gframe.colors[3],
			sframe.depth, redColor, redView, lightProj);

		//////////////////////////
		// Green light!

		// Reuse the shadow pass!
		clearFrameBuffer(sframe);
		tdraw(spass, spear, sframe, spearModel, greenView, lightProj);
		tdraw(spass, sphere, sframe, sphereModel, greenView, lightProj);
		tdraw(spass, quad, sframe, wallModel, greenView, lightProj);
		// add the green light now.
		tdraw(lpass, quad, lframe, camView,
			gframe.colors[0], gframe.colors[1], gframe.colors[2], gframe.colors[3],
			sframe.depth, greenColor, greenView, lightProj);

		////////////////////////////
		// Post Processing

		//Bluring image's glow based on velocity

		//////////////////////////////////////////////////
		// Debug Rendering Stuff. Just single textures to quads-
		// drawing most of the images I've gathered so far.

		// note that the sframe (shadow pass) will only be from the most recent light.

		tdraw(qdraw, quad, screen, lframe.colors[0]);

	}

	context.term();
}