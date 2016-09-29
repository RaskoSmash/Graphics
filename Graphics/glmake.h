#pragma once

struct Geometry;
struct Texture;
struct FrameBuffer;
struct Vertex;
struct Shader;

Geometry makeGeometry(const Vertex *verts, size_t vsize, const unsigned * tris, size_t tsize);
void freeGeometry(Geometry &geo);

Shader makeShader(const char *vsource, const char *fsource,
	bool depth = true, bool add = false, bool face = true);
void freeShader(Shader &shady);

Texture makeTexture(unsigned width, unsigned height, unsigned channels, const void *pixels, bool isFloat = false);
void freeTexture(Texture &);

FrameBuffer makeFrameBuffer(unsigned w, unsigned h, unsigned nColors, const bool *isFloat = nullptr,
	const int *channels = nullptr);
void freeFrameBuffer(FrameBuffer &);


