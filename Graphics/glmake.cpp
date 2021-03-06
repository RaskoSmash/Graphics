#include "gldecs.h"
#include "glmake.h"
#include "globjects.h"
#include "Vertex.h"


Geometry makeGeometry(const Vertex *verts, size_t vsize,
	const unsigned int *tris, size_t tsize)
{
	Geometry retval;

	// Define the variables
	glGenBuffers(1, &retval.vbo);	   // Store vertices
	glGenBuffers(1, &retval.ibo);	   // Store indices
	glGenVertexArrays(1, &retval.vao); // Store attribute information

									   // Scope the variables
	glBindVertexArray(retval.vao);
	glBindBuffer(GL_ARRAY_BUFFER, retval.vbo); // scope our vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, retval.ibo); // triangle is scoped

													   // Initialize the variables
	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize * sizeof(unsigned), tris, GL_STATIC_DRAW);

	// Attributes let us tell openGL how the memory is laid out
	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Color
	glEnableVertexAttribArray(2); // Normal
	glEnableVertexAttribArray(3); // TexCoord

								  // index of the attribute, number of elements, type, normalized?, size of vertex, offset
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::POSITION);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::COLOR);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::NORMAL);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::TEXCOORD);

	// unscope the variables
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	retval.size = tsize;
	return retval;
}

void freeGeometry(Geometry &geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteVertexArrays(1, &geo.vao);
	geo = { 0,0,0,0 };
}


Shader makeShader(const char * vsource, const char * fsource,
	bool depth, bool add, bool face)
{
	Shader retval = { 0, depth, add, face };
	// create our variables
	retval.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);

	// initialize our variables
	glShaderSource(vs, 1, &vsource, 0);
	glShaderSource(fs, 1, &fsource, 0);
	// compile the shaders
	glCompileShader(vs);
	glCompileShader(fs);
	// link the shaders into a single program
	glAttachShader(retval.handle, vs);
	glAttachShader(retval.handle, fs);

	glog_glLinkProgram(retval.handle);
	// no longer need these! Their functionality has been eaten by the program.
	glDeleteShader(vs);
	glDeleteShader(fs);

	return retval;
}

void freeShader(Shader &shader)
{
	glDeleteProgram(shader.handle);
	shader.handle = 0;
}


Texture makeTexture(unsigned width, unsigned height, unsigned channels, const void *pixels, 
	bool isFloat)
{
	GLenum eformat = GL_RGBA; //# of channels
	GLenum iformat = isFloat ? GL_RGBA32F : eformat; //how the channels are being stored 
	//if we are using float then use GL_RGBA32F else use eformat
	switch (channels)
	{
	case 0: eformat = GL_DEPTH_COMPONENT; iformat = GL_DEPTH24_STENCIL8; break;
	case 1: eformat = GL_RED;  iformat = isFloat ? GL_R32F : eformat;  break;
	case 2: eformat = GL_RG;   iformat = isFloat ? GL_RG32F : eformat;  break;
	case 3: eformat = GL_RGB;  iformat = isFloat ? GL_RGB32F : eformat;  break;
	case 4: eformat = GL_RGBA; iformat = isFloat ? GL_RGBA32F : eformat;  break;
	default: glog("ERROR", "Channels must be 0-4");
	}
	//setting eformat & iformat based on channel

	Texture retval = { 0, width, height, channels };

	glGenTextures(1, &retval.handle);				// Declaration
	glBindTexture(GL_TEXTURE_2D, retval.handle);    // Scoping

													// GL_RED, GL_RG, GL_RGB, GL_RGBA
	glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, eformat, isFloat ? GL_FLOAT : GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return retval;
}

void freeTexture(Texture &t)
{
	glDeleteTextures(1, &t.handle);
	t = { 0,0,0,0 };
}


FrameBuffer makeFrameBuffer(unsigned width, unsigned height, unsigned nColors, const bool *isFloat,
	const int *channels)
{
	glog("TODO", "Find a way to implement state management.");
	glog("TODO", "Better implementation of the depth buffer.");
	glog("TODO", "Provide more options? enable/disable stencil/depth buffers.");

	FrameBuffer retval = { 0,width,height,nColors };

	glGenFramebuffers(1, &retval.handle);
	glBindFramebuffer(GL_FRAMEBUFFER, retval.handle);
	////////////////////////////////////////////////////////////////////////////////////
	retval.depth = makeTexture(width, height, 0, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, retval.depth.handle, 0);
	////////////////////////////////////////////////////////////////////////////////////
	const GLenum attachments[8] =
	{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };

	for (int i = 0; i < nColors && i < 8; ++i)
	{
		retval.colors[i] = makeTexture(width, height, channels ? channels[i] : 4, 0, 
			isFloat ? isFloat[i] : false);
		glFramebufferTexture(GL_FRAMEBUFFER, attachments[i],
			retval.colors[i].handle, 0);
	}
	glDrawBuffers(nColors, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return retval;
}


void freeFrameBuffer(FrameBuffer &fb)
{
	for (unsigned i = 0; i < fb.nColors; ++i)
		freeTexture(fb.colors[i]);

	glDeleteFramebuffers(1, &fb.handle);
	fb = { 0,0,0,0 };
}