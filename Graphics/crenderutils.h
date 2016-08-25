#pragma once

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