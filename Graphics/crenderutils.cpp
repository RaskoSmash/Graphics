#define GLEW_STATIC
#include "GLEW\glew.h"
//#include "GLFW\glfw3.h"
#include "vertex.h"
#include "crenderutils.h"

Geometry makeGeometry(const Vertex * vert, size_t vsize, const unsigned * tris, size_t tsize)
{
	Geometry returnVal;
	//define variables
	glCreateBuffers(1, &returnVal.vbo);	//stores vertices
	glCreateBuffers(1, &returnVal.ibo);	//stores indices
	glCreateVertexArrays(1, &returnVal.vao); //stores attribute info
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
