
#include "crenderutils.h"

#include "GLM\ext.hpp"

void main()
{
	Window context;
	context.init(1280, 720);

	Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);
	Geometry spear = loadObj("../res/models/soulspear.obj");
	Geometry cube = loadObj("../res/models/sphere.obj");

	Texture spear_normal = loadTexture("../res/textures/soulspear_normal.tga");
	Texture spear_diffuse = loadTexture("../res/textures/soulspear_diffuse.tga");
	Texture spear_specular = loadTexture("../res/textures/soulspear_specular.tga");

	const unsigned char norm_pixels[4] = { 127, 127, 255, 255 };
	Texture vertex_normals = makeTexture(1, 1, 4, norm_pixels);

	const unsigned char white_pixels[4] = { 255, 255, 255, 255 };
	Texture white = makeTexture(1, 1, 4, white_pixels);

	Shader gpass = loadShader("../res/shaders/gpass.vert",
		"../res/shaders/gpass.frag");

	Shader lpass = loadShader("../res/shaders/lPass.vert",
		"../res/shaders/lPass.frag", false, true);

	Shader post = loadShader("../res/shaders/quad.vert",
		"../res/shaders/quad.frag", false);

	Shader blur = loadShader("../res/shaders/post.vert",
		"../res/shaders/post.frag", false);

	FrameBuffer screen = { 0, 1280, 720 };
	FrameBuffer gframe = makeFrameBuffer(1280, 720, 4);
	FrameBuffer lframe = makeFrameBuffer(1280, 720, 3);
	FrameBuffer nframe = makeFrameBuffer(1280, 720, 1);

	glm::mat4 model, view, proj;

	model = glm::translate(glm::vec3(0, -1, 0));
	view = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	proj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);

	float time = 0;

	while (context.step())
	{
		time += 0.016f;
		clearFrameBuffer(gframe);
		clearFrameBuffer(lframe);
		clearFrameBuffer(nframe);

		model = glm::rotate(time, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0));

		/////////////////////////////////////////////////////
		// Geometry Pass
		//
		// Only the textures, geometry, and model matrix really differs
		// from object to object.
		tdraw(gpass, spear, gframe, model, view, proj,
			spear_diffuse, spear_normal, spear_specular);

		tdraw(gpass, cube, gframe, model, view, proj,
			white, vertex_normals, white);

		tdraw(gpass, quad, gframe,
			glm::rotate(45.f, glm::vec3(0, -1, 0))*
			glm::translate(glm::vec3(0, 0, -2)) *
			glm::scale(glm::vec3(2, 2, 1)),
			view, proj,
			white, vertex_normals, white);

		tdraw(blur, quad, nframe, gframe.colors[1]);


		/////////////////////////////////////////////////////
		//// Lights!
		////
		//// Each call is a different light
		//// They all use the same information from the g-pass,
		//// but provide different colors/directions.

		tdraw(lpass, quad, lframe, view, proj,
			gframe.colors[0], gframe.colors[1],
			gframe.colors[2], gframe.colors[3],
			gframe.depth,
			glm::normalize(glm::vec4(1, -1, -1, 0)), glm::vec4(1, 0, 0, 1));

		tdraw(lpass, quad, lframe, view, proj,
			gframe.colors[0], gframe.colors[1],
			gframe.colors[2], gframe.colors[3],
			gframe.depth,
			glm::normalize(glm::vec4(1, 1, -1, 0)), glm::vec4(0, 1, 0, 1));

		tdraw(lpass, quad, lframe, view, proj,
			gframe.colors[0], gframe.colors[1],
			gframe.colors[2], gframe.colors[3],
			gframe.depth,
			glm::normalize(glm::vec4(-1, -1, 1, 0)), glm::vec4(0, 0, 1, 1));

		//////////////////////////////////////////////////
		// Debug Rendering Stuff. Just single textures to quads-
		// drawing most of the images I've gathered so far.

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

/*	LOL tried to fix camera in my deffered branch

#include "crenderutils.h"
#include "camera.h"
#include "input.h"
#include "timer.h"
#include "GLM\ext.hpp"

void main()
{
#pragma region Init
Window context;
context.init(1280, 720);

Geometry quad = makeGeometry(quad_verts, 4, quad_tris, 6);
Geometry spear = loadObj("../res/models/soulspear.obj");
Geometry cube = loadObj("../res/models/sphere.obj");

Texture spear_normal = loadTexture("../res/textures/soulspear_normal.tga");
Texture spear_diffuse = loadTexture("../res/textures/soulspear_diffuse.tga");
Texture spear_specular = loadTexture("../res/textures/soulspear_specular.tga");

const unsigned char norm_pixels[4] = { 127, 127, 255, 255 };
Texture vertex_normals = makeTexture(1, 1, 4, norm_pixels);

const unsigned char white_pixels[4] = { 255, 255, 255, 255 };
Texture white = makeTexture(1, 1, 4, white_pixels);

Shader gpass = loadShader("../res/shaders/gpass.vert",
"../res/shaders/gpass.frag");

Shader lpass = loadShader("../res/shaders/lPass.vert",
"../res/shaders/lPass.frag", false, true);

Shader post = loadShader("../res/shaders/quad.vert",
"../res/shaders/quad.frag", false);

Shader blur = loadShader("../res/shaders/post.vert",
"../res/shaders/post.frag", false);

FrameBuffer screen = { 0, 1280, 720 };
FrameBuffer gframe = makeFrameBuffer(1280, 720, 4);
FrameBuffer lframe = makeFrameBuffer(1280, 720, 3);
FrameBuffer nframe = makeFrameBuffer(1280, 720, 1);


float timer = 0;
#pragma endregion

Timer time;
Input input;
FlyCamera camera;
camera.jumpTo(glm::vec3(0, 0, 0));
camera.lookAt(glm::vec3(0, 0, 0));

time.init();
input.init(context);

glm::mat4 model, view, proj;

model = glm::translate(glm::vec3(0, -1, 0));
view = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
proj = glm::perspective(45.f, 1280.f / 720, 1.f, 100.f);
while (context.step())
{
timer += time.getDeltaTime();
clearFrameBuffer(gframe);
clearFrameBuffer(lframe);
clearFrameBuffer(nframe);

view = camera.getView();
proj = camera.getProjection();

camera.update(input, time);

model = glm::rotate(timer, glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(0, -1, 0));

/////////////////////////////////////////////////////
// Geometry Pass
//
// Only the textures, geometry, and model matrix really differs
// from object to object.
tdraw(gpass, spear, gframe, model, view, proj,
spear_diffuse, spear_normal, spear_specular);

tdraw(gpass, cube, gframe, model, view, proj,
white, vertex_normals, white);

tdraw(gpass, quad, gframe,
glm::rotate(45.f, glm::vec3(0, -1, 0))*
glm::translate(glm::vec3(0, 0, -2)) *
glm::scale(glm::vec3(2, 2, 1)),
view, proj,
white, vertex_normals, white);

tdraw(blur, quad, nframe, gframe.colors[1]);


/////////////////////////////////////////////////////
//// Lights!
////
//// Each call is a different light
//// They all use the same information from the g-pass,
//// but provide different colors/directions.

tdraw(lpass, quad, lframe, view, proj,
gframe.colors[0], gframe.colors[1],
gframe.colors[2], gframe.colors[3],
gframe.depth,
glm::normalize(glm::vec4(1, -1, -1, 0)), glm::vec4(1, 0, 0, 1));

tdraw(lpass, quad, lframe, view, proj,
gframe.colors[0], gframe.colors[1],
gframe.colors[2], gframe.colors[3],
gframe.depth,
glm::normalize(glm::vec4(1, 1, -1, 0)), glm::vec4(0, 1, 0, 1));

tdraw(lpass, quad, lframe, view, proj,
gframe.colors[0], gframe.colors[1],
gframe.colors[2], gframe.colors[3],
gframe.depth,
glm::normalize(glm::vec4(-1, -1, 1, 0)), glm::vec4(0, 0, 1, 1));

//////////////////////////////////////////////////
// Debug Rendering Stuff. Just single textures to quads-
// drawing most of the images I've gathered so far.

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
input.term();
time.term();
context.term();
}

*/