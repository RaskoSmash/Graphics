#pragma once

struct Geometry;
struct Texture;
struct FrameBuffer;
struct Vertex;
struct Shader;

Geometry makeGeometry(const Vertex *verts, size_t vsize, const unsigned * tris, size_t tsize);
void freeGeometry(Geometry &geo);

Shader makeShader(const char *vsource, const char *fsource);
void freeShader(Shader &shady);

Texture makeTexture(unsigned width, unsigned height, unsigned format, const unsigned char *pixels);	//makes an image
Texture makeTextureF(unsigned sqr, const float *pixels);	//for pure data
void freeTexture(Texture &);

FrameBuffer makeFrameBuffer(unsigned w, unsigned h, unsigned nColors);
void freeFrameBuffer(FrameBuffer &);


