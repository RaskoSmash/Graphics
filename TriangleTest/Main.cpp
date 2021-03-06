
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
	
	//TODO: Illumination model


	Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);
	Geometry spear = loadObj("../res/models/soulspear.obj");
	Geometry sphere = loadObj("../res/models/sphere.obj");

	Texture spear_normal = loadTexture("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = loadTexture("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = loadTexture("../res/textures/soulspear_specular.tga");
	Texture meleetex = loadTexture("../res/textures/melee.jpg");

	Texture spheretex = makeHeightMap(128,32);
	Geometry plane = makeGrid(512, 20);


	const unsigned char norm_pixels[4] = { 127, 127, 255, 255 };
	Texture vertex_normals = makeTexture(1, 1, 4, norm_pixels);

	const unsigned char white_pixels[4] = { 255, 255, 255, 255 };
	Texture white = makeTexture(1, 1, 4, white_pixels);

	Shader glowpass = loadShader("../res/shaders/glowpass.vert", "../res/shaders/glowpass.frag");
	Shader qdraw = loadShader("../res/shaders/quad.vert", "../res/shaders/quad.frag", false);
	Shader gpass = loadShader("../res/shaders/gpass.vert", "../res/shaders/gpass.frag");

	//////////////////////////////
	//// Using Light pass shader designed for shadows.
	//// Note that shadow pass can disable face-culling for some back-shadow improvements.
	Shader spass = loadShader("../res/shaders/spass.vert", "../res/shaders/spass.frag", true, false, false);
	Shader lpass = loadShader("../res/shaders/lspass.vert", "../res/shaders/lspass.frag", false, true);

	Shader post = loadShader("../res/shaders/post.vert", "../res/shaders/post.frag");

	FrameBuffer glowF = makeFrameBuffer(1280, 720, 3); //makes glow map

	FrameBuffer screen = { 0, 1280, 720 };
	FrameBuffer aster = makeFrameBuffer(720,720,1);

	bool flTex[] = { false, true, false, true }; // colors don't need floats, but position/normal should use it.
	FrameBuffer gframe = makeFrameBuffer(1280, 720, 4, flTex);
	FrameBuffer lframe = makeFrameBuffer(1280, 720, 3);


	// Temporary shadow framebuffer. Will be cleared and reused by each light!
	// Its RESOLUTION WILL GREATLY EFFECT THE QUALITY. Try playing around with high/low res.
	FrameBuffer sframe = makeFrameBuffer(1024, 1024, 0);

	FrameBuffer pframe = makeFrameBuffer(1280, 720, 1);


	// Camera information
	glm::mat4 camView = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	cam.lookAt(glm::vec3(camView[0][3], camView[1][3], camView[2][3]));
	glm::mat4 camProj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	// Model Matrices
	glm::mat4 spearModel; // ROTATES in main
	glm::mat4 sphereModel = glm::translate(glm::vec3(0.3f, -1, -0.2f));
	glm::mat4 wallModel = glm::rotate(45.f, glm::vec3(0, -1, 0)) * 
		glm::translate(glm::vec3(0, 0, -6)) * glm::scale(glm::vec3(10, 10, 1));

	glm::mat4 planeModel = glm::rotate(45.f, glm::vec3(0, -1, 0)) *
		glm::translate(glm::vec3(20, 0, 0));

	//glm::mat4 wallModel = glm::translate(glm::vec3(0.3f, -1, -0.2f));

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
	float mountScale = 0;;
	bool isFailure = 0;

	while (context.step())
	{
		time.step();
		input.step();

		if (input.getKeyState('9') == Input::DOWN) isFailure = true;
		if (input.getKeyState('0') == Input::DOWN) isFailure = false;

		times += time.getDeltaTime();
		camView = cam.getView();
		camProj = cam.getProjection();

		mountScale = sin(times) * 4;
		cam.update(input, time);

		spearModel = glm::rotate(times, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0));
		sphereModel = glm::translate(glm::vec3(0, 0, cosf(times) * 5));
		sphereModel *= glm::rotate(times, glm::vec3(0, 1, 0));
		/////////////////////////////////////////////////////
		// Geometry Pass
		//	TODO: GLOW MAP
		clearFrameBuffer(gframe);
		clearFrameBuffer(glowF);
		tdraw(gpass, spear, gframe, spearModel, camView, camProj, spear_diffuse, spear_normal, spear_specular, false);
		tdraw(gpass, sphere, gframe, sphereModel, camView, camProj, spheretex, vertex_normals, white, false);
		tdraw(gpass, quad, gframe, wallModel, camView, camProj, white, vertex_normals, white, false);
		tdraw(gpass, plane, gframe, planeModel, camView, camProj, meleetex, vertex_normals, white, true, meleetex, mountScale);
		tdraw(glowpass, quad, glowF, gframe.colors[0], 0.2f, 1.0f, 1.0f);

		//tdraw(blur, quad, nframe, gframe.colors[1]);

		/////////////////////////////////////////////////////
		//// Light pass!
		clearFrameBuffer(lframe);

		//////////////////////////
		// RED LIGHT

		// Shadow PrePass
		// TODO: ADD VELOCITY STUFF
		clearFrameBuffer(sframe);
		tdraw(spass, spear, sframe, spearModel, redView, lightProj);
		tdraw(spass, sphere, sframe, sphereModel, redView, lightProj);
		tdraw(spass, quad, sframe, wallModel, redView, lightProj);
		// Light Aggregation
		tdraw(lpass, quad, lframe, camView,
			gframe.colors[0], gframe.colors[1], gframe.colors[2], gframe.colors[3],
			sframe.depth, redColor, redView, lightProj, isFailure);

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
			sframe.depth, greenColor, greenView, lightProj, isFailure);

		////////////////////////////
		// Post Processing
		clearFrameBuffer(pframe);
		tdraw(post, quad, pframe, glowF.colors[0], lframe.colors[0]);
		//Bluring image's glow based on velocity
		// note that the sframe (shadow pass) will only be from the most recent light.

		tdraw(qdraw, quad, screen, pframe.colors[0]);
	}

	context.term();
}