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

	bool depthTest;
	bool faceCulling;
	bool additiveBlend;
};

struct Texture
{
	unsigned handle, width, height, format;
};

struct FrameBuffer
{
	unsigned handle;
	unsigned width, height, nColors;

	Texture depth;
	Texture colors[8];
};