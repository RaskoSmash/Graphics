#include "gldecs.h"
#include "vertex.h"
#include "crenderutils.h"

Geometry makeGeometry(const Vertex * vert, size_t vsize, const unsigned * tris, size_t tsize)
{
	Geometry returnVal;
	//define variables
	glGenBuffers(1, &returnVal.vbo);	//stores vertices
	glGenBuffers(1, &returnVal.ibo);	//stores indices
	glGenVertexArrays(1, &returnVal.vao); //stores attribute info
	//scope the variables
	glBindVertexArray(returnVal.vao);
	glBindBuffer(GL_ARRAY_BUFFER, returnVal.vbo);	//scope out vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, returnVal.ibo); //triangle is scoped
	//initialize the vars
	new float[4];
	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), vert, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize * sizeof(unsigned), tris, GL_STATIC_DRAW);
	//attributes/memory layout
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)16);
	//unscope vars
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	returnVal.size = tsize;
	return returnVal;
}

void freeGeometry(Geometry & geo)
{
	glDeleteBuffers(1, &geo.vbo);
	glDeleteBuffers(1, &geo.ibo);
	glDeleteBuffers(1, &geo.vao);
	geo = { 0,0,0,0 };
}

Shader makeShader(const char * vsource, const char * fsource)
{
	Shader returnVal;
	//create our variables
	returnVal.handle = glCreateProgram();
	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);
	//initialize
	glShaderSource(vs, 1, &vsource, 0);
	glShaderSource(fs, 1, &fsource, 0);
	//compile
	glCompileShader(vs);
	glCompileShader(fs);
	//link shaders into a single program
	glAttachShader(returnVal.handle, vs);
	glAttachShader(returnVal.handle, fs);
	glLinkProgram(returnVal.handle);
	//no longer need these
	glDeleteShader(vs);
	glDeleteShader(fs);

	return returnVal;
}

void freeShader(Shader & shady)
{
	glDeleteProgram(shady.handle);
	shady.handle = 0;
}

void draw(const Shader &shady, const Geometry &geo)
{

	glUseProgram(shady.handle);

	//binding the VAO also binds IBO(tri) and VBO (verts)
	glBindVertexArray(geo.vao);

	//Draw elements will draw the verts that are currently bound
	//using an array of indices.
	//IF AN IBO IS BOUND, we dont need to provide indices.
	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}