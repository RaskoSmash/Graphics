#include "Gallery.h"

bool Gallery::makeShader(const char * name, const char * vsource, const char * fsource)
{
	//check if name already exists
	shaders[name] = ::makeShader(vsource, fsource);
	return true;
}

bool Gallery::loadShader(const char * name, const char * vpath, const char * fpath)
{
	shaders[name] = ::loadShader(vpath, fpath);
	return true;
}

bool Gallery::makeObject(const char * name, const Vertex * verts, size_t vsize, const unsigned * tris, size_t tsize)
{
	objects[name] = ::makeGeometry(verts, vsize, tris, tsize);
	return true;
}

bool Gallery::loadObjectOBJ(const char * name, const char * path)
{
	objects[name] = ::loadObj(path);
	return true;
}

const Geometry & Gallery::getObject(const char * name)
{
	return objects[name];
}

const Shader & Gallery::getShader(const char * name)
{
	// need to validate name first
	return shaders[name];
}

bool Gallery::init()
{
	loadShader("defaultShader", "../res/shaders/simpleVert.txt", "../res/shaders/simpleFrag.txt");
	loadObjectOBJ("sphere","../res/models/sphere.obj");
	return true;
}

bool Gallery::term()
{
	//loop through all maps and call free functions
	for each (auto shader in shaders)
	{
		freeShader(shader.second);
	}

	for each (auto object in objects)
	{
		freeGeometry(object.second);
	}
	return true;
}
