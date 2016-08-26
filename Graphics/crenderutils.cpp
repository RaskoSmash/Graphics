#include "gldecs.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "OBJ\tiny_obj_loader.h"
#include "Vertex.h"
#include "crenderutils.h"

#include <cstdio>

Geometry makeGeometry(const Vertex       * verts, size_t vsize,
	const unsigned int * tris, size_t tsize)
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
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// index of the attribute, number of elements, type, normalized?, size of vertex, offset
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::POSITION);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::COLOR);

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

Shader makeShader(const char * vsource, const char * fsource)
{
	Shader retval;
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
	glLinkProgram(retval.handle);
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

void draw(const Shader &shader, const Geometry &geometry)
{
	glUseProgram(shader.handle);

	// binding the VAO also binds the IBO (tri) and VBO (verts)
	glBindVertexArray(geometry.vao);

	// Draw elements will draw the vertices that are currently bound
	// using an array of indices.
	// IF AN IBO IS BOUND, we don't need to provide any indices.
	glDrawElements(GL_TRIANGLES, geometry.size, GL_UNSIGNED_INT, 0);
}

// return the number of characters in the file
long copyFileToArray(char *dest, size_t dlen, const char *path)
{
	FILE *fp;
	fopen_s(&fp, path, "r");	// open file	
	if (!fp) return 0;

	fseek(fp, 0L, SEEK_END);		// jump to end of file
	long size = ftell(fp);			// determine how many chars we passed
	rewind(fp);						// go back to beginning

	long lastIndex = fread_s(dest, dlen, size, 1, fp);

	dest[lastIndex] = 0;

	fclose(fp);
	return size;
}
#include <fstream>
#include <istream>
#include <string>

// not quite working!
std::string cppStyleFileToString(const char *path)
{
	std::ifstream infile{ path };
	std::string file_contents{ std::istreambuf_iterator<char>(infile),
		std::istreambuf_iterator<char>() };
	return file_contents;
}

Geometry loadObj(const char * path)
{
	tinyobj::attrib_t attrib;

	std::vector<tinyobj::shape_t> shapes;
	return Geometry();
}

Shader loadShader(const char *vpath, const char *fpath)
{
	//char vsource[5012]; //max of 5012 characters in source
	//char fsource[5012];	//will probably change that later

	//copyFileToArray(vsource, 5012, vpath);
	//copyFileToArray(fsource, 5012, fpath);

	//
	//return makeShader(vsource, fsource);

	std::string vs = cppStyleFileToString(vpath);
	std::string fs = cppStyleFileToString(fpath);

	return makeShader(vs.c_str(), fs.c_str());
}

//mine that doesnt work

//#include "gldecs.h"
//#include "vertex.h"
//#include "crenderutils.h"
//#include <iostream>
//#include <fstream>
//
//Geometry makeGeometry(const Vertex * vert, size_t vsize, const unsigned * tris, size_t tsize)
//{
//	Geometry returnVal;
//	//define variables
//	glGenBuffers(1, &returnVal.vbo);	//stores vertices
//	glGenBuffers(1, &returnVal.ibo);	//stores indices
//	glGenVertexArrays(1, &returnVal.vao); //stores attribute info
//										  //scope the variables
//	glBindVertexArray(returnVal.vao);
//	glBindBuffer(GL_ARRAY_BUFFER, returnVal.vbo);	//scope out vertices
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, returnVal.ibo); //triangle is scoped
//														  //initialize the vars
//	new float[4];
//	glBufferData(GL_ARRAY_BUFFER, vsize * sizeof(Vertex), vert, GL_STATIC_DRAW);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize * sizeof(unsigned), tris, GL_STATIC_DRAW);
//	//attributes/memory layout
//	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)16);
//	//unscope vars
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	returnVal.size = tsize;
//	return returnVal;
//}
//
//void freeGeometry(Geometry & geo)
//{
//	glDeleteBuffers(1, &geo.vbo);
//	glDeleteBuffers(1, &geo.ibo);
//	glDeleteBuffers(1, &geo.vao);
//	geo = { 0,0,0,0 };
//}
//
//Shader makeShader(const char * vsource, const char * fsource)
//{
//	Shader returnVal;
//	//create our variables
//	returnVal.handle = glCreateProgram();
//	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
//	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);
//	//initialize
//	glShaderSource(vs, 1, &vsource, 0);
//	glShaderSource(fs, 1, &fsource, 0);
//	//compile
//	glCompileShader(vs);
//	glCompileShader(fs);
//	//link shaders into a single program
//	glAttachShader(returnVal.handle, vs);
//	glAttachShader(returnVal.handle, fs);
//	glLinkProgram(returnVal.handle);
//	//no longer need these
//	glDeleteShader(vs);
//	glDeleteShader(fs);
//
//	return returnVal;
//}
//
//bool getTextFromFile(char *dest, size_t size, const char *pathToFile)
//{
//	//load file into char pointer
//	std::fstream myFile;
//	myFile.open(pathToFile);
//
//	if (myFile.is_open())
//	{
//		while (!myFile.eof())
//		{
//			myFile >> dest;
//		}
//	}
//	myFile.close();
//	return true;
//}
//
//Shader loadShader(const char * vpath, const char * fpath)
//{
//	char vsource[5012];
//	char fsource[5012];
//
//	getTextFromFile(vsource, 5012, vpath);
//	getTextFromFile(fsource, 5012, fpath);
//
//	return makeShader(vsource, fsource);
//}
//
//void freeShader(Shader & shady)
//{
//	glDeleteProgram(shady.handle);
//	shady.handle = 0;
//}
//
//void draw(const Shader &shady, const Geometry &geo)
//{
//
//	glUseProgram(shady.handle);
//
//	//binding the VAO also binds IBO(tri) and VBO (verts)
//	glBindVertexArray(geo.vao);
//
//	//Draw elements will draw the verts that are currently bound
//	//using an array of indices.
//	//IF AN IBO IS BOUND, we dont need to provide indices.
//	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
//}