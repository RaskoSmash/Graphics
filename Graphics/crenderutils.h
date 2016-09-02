#pragma once
#include <iostream>
#include "globjects.h"
struct Geometry;

struct Vertex;

struct Shader;

struct Texture;

Geometry makeGeometry(const Vertex *verts, size_t vsize, const unsigned * tris, size_t tsize);

void freeGeometry(Geometry &geo);

Shader makeShader(const char *vsource, const char *fsource);

long copyFileToArray(char *dest, size_t dlen, const char *path);

Shader loadShader(const char *vpath, const char *fpath);

void freeShader(Shader &shady);

void draw(const Shader &shady, const Geometry &geo);
void draw(const Shader &shady, const Geometry &geo, float time);
void draw(const Shader &shady, const Geometry &geo, 
	const float M[16], const float V[16], const float P[16], float time);
void draw(const Shader &shady, const Geometry &geo, 
	const float M[16], const float V[16], const float P[16], float time, const class Input &in);

std::string cppStyleFileToString(const char *path);

Geometry loadObj(const char *path);

Texture makeTexture(unsigned width, unsigned height, unsigned format, const unsigned *pixels);

void freeTexture(Texture &);

void draw(const Shader &shady, const Geometry &geo, Texture &t,
	const float M[16], const float V[16], const float P[16], float time);