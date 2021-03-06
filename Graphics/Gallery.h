#pragma once

#include <string>
#include <map>
#include "crenderutils.h"


class Gallery
{
	std::map<std::string, Geometry> objects;
	std::map<std::string, Shader> shaders;
	std::map<std::string, Texture> textures;

public:
	bool makeShader(const char* name, const char* vsource, const char *fsource);
	bool loadShader(const char* name, const char* vpath, const char *fpath);

	bool makeObject(const char* name, const Vertex* verts, size_t vsize, const unsigned *tris, size_t tsize);
	bool loadObjectOBJ(const char *name, const char *path);

	bool makeTexture(const char * name, unsigned width, unsigned height, unsigned format, const unsigned char *pixels);
	bool loadTexture(const char *name, const char *path);

	const Geometry &getObject(const char *name);
	const Shader   &getShader(const char *name);
	const Texture  &getTexture(const char *name);

	bool init();
	bool term();
};