#include "crenderutils.h"

#include "GLM\ext.hpp"

void main()
{
	Window context;
	context.init(1280, 720);

	Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);
	Geometry spear = loadObj("../res/models/soulspear.obj");

	Texture spear_normal = loadTexture("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = loadTexture("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = loadTexture("../res/textures/soulspear_specular.tga");


	Shader simple = loadShader("../res/shaders/simpleVert.vert",
		"../res/shaders/simpleFrag.frag");

	Shader post = loadShader("../res/shaders/post.vert",
		"../res/shaders/post.frag");

	FrameBuffer screen = { 0,1280,720 };
	FrameBuffer frame = makeFrameBuffer(1280, 720, 2);

	glm::mat4 model, view, proj;

	model = glm::translate(glm::vec3(0, -1, 0));
	view = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	proj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	float time = 0;

	while (context.step())
	{
		time += 0.016f;
		clearFrameBuffer(frame);

		model = glm::rotate(time, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0));

		tdraw(simple, spear, frame, model, view, proj,
			spear_diffuse, spear_normal, spear_specular);

		tdraw(post, quad, screen, frame.colors[0], frame.colors[1]);
	}

	context.term();
}