#include "gldecs.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "OBJ\tiny_obj_loader.h"
#include "Vertex.h"
#include "input.h"
#include "crenderutils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB\stb_image.h"
#include <cstdio>
#include "GLM\gtc\noise.hpp"

Geometry makeGeometry(const Vertex * verts, size_t vsize,
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

void draw(const Shader &shady, const Geometry &geo)
{
	glUseProgram(shady.handle);

	// binding the VAO also binds the IBO (tri) and VBO (verts)
	glBindVertexArray(geo.vao);

	// Draw elements will draw the vertices that are currently bound
	// using an array of indices.
	// IF AN IBO IS BOUND, we don't need to provide any indices.
	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}

void draw(const Shader & shady, const Geometry & geo, float time)
{
	glUseProgram(shady.handle);

	glBindVertexArray(geo.vao);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int loc = glGetUniformLocation(shady.handle, "time");
	glUniform1f(loc, time);

	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}

void draw(const Shader & shady, const Geometry & geo, const float M[16], const float V[16], const float P[16], float time)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shady.handle);
	glBindVertexArray(geo.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);

	glUniform1f(3, time);

	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}

void draw(const Shader & shady, const Geometry & geo, 
	const float M[16], const float V[16], const float P[16], float time, const Input &in)
{
	glEnable(GL_CULL_FACE); 
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shady.handle);
	glBindVertexArray(geo.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);

	glUniform1f(3, time);

	for (int i = 0; i < 350; ++i)
	{
		if (in.getKeyState(i) == Input::DOWN)
		{
			glUniform1i(4, i);
		}
	}


	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
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
	std::vector<tinyobj::material_t> materials;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);

	Vertex   *verts = new Vertex[attrib.vertices.size() / 3];
	unsigned * tris = new unsigned[shapes[0].mesh.indices.size()];

	for (int i = 0; i < attrib.vertices.size() / 3; ++i)
	{
		verts[i] = { attrib.vertices[i * 3],
			attrib.vertices[i * 3 + 1],
			attrib.vertices[i * 3 + 2], 1 };

		verts[i].color[0] = rand() * 1.0f / RAND_MAX;
		verts[i].color[1] = rand() * 1.0f / RAND_MAX;
		verts[i].color[2] = rand() * 1.0f / RAND_MAX;
		verts[i].color[3] = 1;
	}

	for (int i = 0; i < shapes[0].mesh.indices.size(); ++i)
		tris[i] = shapes[0].mesh.indices[i].vertex_index;

	Geometry retval = makeGeometry(verts, attrib.vertices.size() / 3,
		tris, shapes[0].mesh.indices.size());

	delete[] verts;
	delete[] tris;
	// then we can call makeGeometry as per normal.
	return retval;
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

Texture makeTexture(unsigned width, unsigned height, unsigned format, const unsigned char *pixels)
{
	Texture retval = { 0,width,height,format };

	glGenTextures(1, &retval.handle);	//declaration
	glBindTexture(GL_TEXTURE_2D, retval.handle);	//scoping

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	return retval;
}
Texture loadTexture(const char *path)
{
	int w, h, f;
	unsigned char *p;

	Texture retval = {0,0,0,0};

	p = stbi_load(path, &w, &h, &f, STBI_default);
	if(!p) return retval;

	switch (f)
	{
	case STBI_grey:
		f = GL_RED;
		break;
	case STBI_grey_alpha:
		f = GL_RG;
		break;
	case STBI_rgb:
		f = GL_RGB;
		break;
	case STBI_rgb_alpha:
		f = GL_RGBA;
		break;
	}

	retval = makeTexture(w, h, f, p);
	stbi_image_free(p);
	return retval;
}
void freeTexture(Texture &t)
{
	glDeleteTextures(1, &t.handle);
	t = {0,0,0,0};
}
void draw(const Shader & shady, const Geometry & geo, const Texture & t, 
	const float M[16], const float V[16], const float P[16], float time)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUseProgram(shady.handle);
	glBindVertexArray(geo.vao);

	glUniformMatrix4fv(0, 1, GL_FALSE, P);
	glUniformMatrix4fv(1, 1, GL_FALSE, V);
	glUniformMatrix4fv(2, 1, GL_FALSE, M);

	glUniform1f(3, time);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t.handle);
	glUniform1i(4, 0);

	glDrawElements(GL_TRIANGLES, geo.size, GL_UNSIGNED_INT, 0);
}

Geometry generateGrid(unsigned int rows, unsigned int cols)
{
	Geometry retval;
	Vertex *aoVertices = new Vertex[rows * cols];

	for(unsigned int r = 0; r < rows; ++r)
		for (unsigned int c = 0; c < cols; ++c)
		{
			float vec4[] = { (float)c,0,(float)r,1 };
			memcpy_s(&aoVertices[r * cols + c], sizeof(float) * 4, &vec4, sizeof(float) * 4);
			//aoVertices[r * cols + c].position[0] = vec4[0];
			//aoVertices[r * cols + c].position[1] = vec4[1];
			//aoVertices[r * cols + c].position[2] = vec4[2];
			//aoVertices[r * cols + c].position[3] = vec4[3];
		}

	unsigned int *auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;

	for(unsigned int r = 0; r < (rows - 1); ++r)
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			//triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			//triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}

	retval = makeGeometry(aoVertices, rows * cols, auiIndices, (rows - 1) * (cols - 1) * 6);

	delete[] auiIndices;
	delete[] aoVertices;
	return retval;
}

Texture generateHeightMap(int dims)
{
	float *perlin_data = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	for(int x = 0; x < dims; ++x)
		for (int y = 0; y < dims; ++y)
		{
			perlin_data[y* dims + x] = glm::perlin(glm::vec2(x, y) * scale) * 0.5f + 0.5f;
		}
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