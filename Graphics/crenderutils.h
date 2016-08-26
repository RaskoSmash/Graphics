#pragma once
#include <iostream>

struct Geometry
{
	//vertex buffer object: array of verts
	//index buffer object: array of indicies(triangles)
	//vertex array object: groups the two with formatting
	// size: # of tris
	unsigned vao, vbo, ibo, size;
};

struct Vertex;

Geometry makeGeometry(const Vertex *verts, size_t vsize, const unsigned * tris, size_t tsize);

void freeGeometry(Geometry &geo);

struct Shader
{
	unsigned handle;
};

Shader makeShader(const char *vsource, const char *fsource);

long copyFileToArray(char *dest, size_t dlen, const char *path);

Shader loadShader(const char *vpath, const char *fpath);

void freeShader(Shader &shady);

void draw(const Shader &shady, const Geometry &geo);

std::string cppStyleFileToString(const char *path);

Geometry loadObj(const char *path);