#pragma once

#include "vertex.h"
#include "window.h"
#include "glload.h"
#include "glmake.h"
#include "globjects.h"
#include "gldraw.h"
/*
LETS NOT DELETE STUFF!
#include "globjs.h"
Geometry makeGeometry(const struct Vertex *verts, size_t vsize,
const unsigned int  *tris,  size_t tsize);
void freeGeometry(Geometry &);
Geometry loadOBJ(const char *path);
Shader makeShader(const char *vsource, const char *fsource);
Shader loadShader(const char *vpath, const char *fpath);
void freeShader(Shader &);
void draw(const Shader &, const Geometry &);
void draw(const Shader &, const Geometry &, float time);
void draw(const Shader &, const Geometry &, const float M[16], const float V[16], const float P[16], float time);
// 0-255, #00-#FF
/// R   G  B  A
// #FF FF FF
Texture makeTexture(unsigned width, unsigned height, unsigned format, const unsigned char *pixels);
Texture makeTextureF(unsigned square, const float *pixels);
Texture loadTexture(const char *path);
void freeTexture(Texture &);
void draw(const Shader &, const Geometry &, const Texture &, const float M[16], const float V[16], const float P[16], float time);
void drawPhong(const Shader &s, const Geometry &g,
const float M[16], const float V[16], const float P[16]);
void drawPhong(const Shader &s, const Geometry &g,
const float M[16], const float V[16], const float P[16],
const Texture *T, unsigned t_count);
Framebuffer makeFramebuffer(unsigned width, unsigned height,
unsigned nColors);
void freeFramebuffer(Framebuffer &);
void clearFramebuffer(const Framebuffer &);
void drawFB(const Shader &s, const Geometry &g, const Framebuffer &f,
const float M[16], const float V[16], const float P[16],
const Texture *T, unsigned t_count);
*/