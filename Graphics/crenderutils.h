#pragma once
#include <iostream>
#include "globjects.h"
struct Geometry;

struct Vertex;

struct Shader;

struct Texture;


long copyFileToArray(char *dest, size_t dlen, const char *path);

Shader loadShader(const char *vpath, const char *fpath);

void draw(const Shader &shady, const Geometry &geo);
void draw(const Shader &shady, const Geometry &geo, float time);
void draw(const Shader &shady, const Geometry &geo, 
	const float M[16], const float V[16], const float P[16], float time);
void draw(const Shader &shady, const Geometry &geo, 
	const float M[16], const float V[16], const float P[16], float time, const class Input &in);

std::string cppStyleFileToString(const char *path);

Geometry loadObj(const char *path);


Texture loadTexture(const char *path);


void draw(const Shader &shady, const Geometry &geo, const Texture &t,
	const float M[16], const float V[16], const float P[16], float time);

Geometry generateGrid(unsigned int rows, unsigned int cols);

Texture generateHeightMap(int dims);

void drawPhong(const Shader & shady, const Geometry & geo,
	const float M[16], const float V[16], const float P[16]);

void drawPhong(const Shader & shady, const Geometry & geo,
	const float M[16], const float V[16], const float P[16],
	const Texture *T, unsigned t_count);


void clearFrameBuffer(const FrameBuffer &);

void drawFB(const Shader & shady, const Geometry & geo, FrameBuffer &fb,
	const float M[16], const float V[16], const float P[16],
	const Texture *T, unsigned t_count);