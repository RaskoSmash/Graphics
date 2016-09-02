#pragma once
struct Geometry
{
	//vertex buffer object: array of verts
	//index buffer object: array of indicies(triangles)
	//vertex array object: groups the two with formatting
	// size: # of tris
	unsigned vao, vbo, ibo, size;
};

struct Shader
{
	unsigned handle;
};

struct Texture
{
	unsigned handle, width, height, format;
};