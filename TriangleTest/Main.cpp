
#include "crenderutils.h"

#include "GLM\ext.hpp"

void main()
{
	Window context;
	context.init(1280, 720);

	Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);
	Geometry spear = loadObj("../res/models/soulspear.obj");
	Geometry cube = loadObj("../res/models/cube.obj");

	Texture spear_normal = loadTexture("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = loadTexture("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = loadTexture("../res/textures/soulspear_specular.tga");


	Shader gpass = loadShader("../res/shaders/gpass.vert",
		"../res/shaders/gpass.frag");

	Shader lpass = loadShader("../res/shaders/lpass.vert",
		"../res/shaders/lpass.frag", false,true);

	Shader post = loadShader("../res/shaders/quad.vert",
		"../res/shaders/quad.frag",false);

	Shader blur = loadShader("../res/shaders/post.vert",
		"../res/shaders/post.frag",false);

	FrameBuffer screen = { 0, 1280, 720 };
	FrameBuffer gframe = makeFrameBuffer(1280, 720, 4);
	FrameBuffer lframe = makeFrameBuffer(1280, 720, 3);
	FrameBuffer nframe = makeFrameBuffer(1280, 720, 1);

	glm::mat4 model1, model2, view, proj;

	model1 = glm::translate(glm::vec3(0, -1, 0));

	model2 = glm::translate(glm::vec3(1, 0, 0));

	view = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	proj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	float time = 0;

	while (context.step())
	{
		time += 0.016f;
		clearFrameBuffer(gframe);
		clearFrameBuffer(lframe);
		clearFrameBuffer(nframe);

		model1 = glm::rotate(time, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0));

		model2 = glm::rotate(time, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(1, 0, 0));

		// Geometry Pass
		tdraw(gpass, spear, gframe, model1, view, proj,
			spear_diffuse, spear_normal, spear_specular);

		tdraw(gpass, cube, gframe, model2, view, proj,
			spear_diffuse, spear_normal, spear_specular);

		//blur effect
		tdraw(blur, quad, nframe, gframe.colors[1]);

		// Lighting pass
		tdraw(lpass, quad, lframe, view, proj,
			gframe.colors[0], nframe.colors[0],
			gframe.colors[2], gframe.colors[3],
			gframe.depth);

		// Debug Rendering Stuff.
		for (int i = 0; i < 4; ++i)
		{
			glm::mat4 mod =
				glm::translate(glm::vec3(-.75f + .5*i, 0.75f, 0)) *
				glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
			tdraw(post, quad, screen, gframe.colors[i], mod);
		}

		glm::mat4 mod =
			glm::translate(glm::vec3(-.75f, 0.25f, 0)) *
			glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
		tdraw(post, quad, screen, gframe.depth, mod);

		mod =
			glm::translate(glm::vec3(-.5f, -0.5f, 0)) *
			glm::scale(glm::vec3(0.5f, 0.5f, 1.f));
		tdraw(post, quad, screen, lframe.colors[0], mod);

		mod =
			glm::translate(glm::vec3(.25f, 0.25f, 0)) *
			glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
		tdraw(post, quad, screen, lframe.colors[1], mod);

		mod =
			glm::translate(glm::vec3(.75f, 0.25f, 0)) *
			glm::scale(glm::vec3(0.25f, 0.25f, 1.f));
		tdraw(post, quad, screen, lframe.colors[2], mod);
	}

	context.term();
}