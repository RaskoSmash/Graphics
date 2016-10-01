#include "generation.h"
#include "crenderutils.h"
#include "GLM\gtc\noise.hpp"

Texture makeHeightMap(int sqr, unsigned octaves)
{
	Texture texturr;
	float *noise = new float[sqr * sqr];
	float scale = (1.0f / sqr) * 3.f;
		for (int x = 0; x < sqr; ++x)
		{
			for (int y = 0; y < sqr; ++y)
			{
			float amplitude = 1.f;
			float persistence = .25f;
			noise[y* sqr + x] = glm::perlin(glm::vec2(x, y) * scale) * 0.5f + 0.5f;
				for (int o = 0; o < octaves; ++o)
				{
					float freq = powf(2, o);
					float data = glm::perlin(glm::vec2(x, y)*scale*freq) * 0.5f + 0.5f;
					noise[y*sqr + x] += data * amplitude;
					amplitude *= persistence;
				}
			}
		}
	texturr = makeTexture(sqr, sqr, 0, noise, true);
	delete[] noise;
	return texturr;

}

Geometry makeGrid(unsigned int sqr, float dim)
{

	Geometry geo;
	unsigned vsize = sqr * sqr;
	unsigned tsize = (sqr - 1) * (sqr - 1) * 6;
	Vertex *verts = new Vertex[vsize];
	unsigned int *tris = new unsigned int[tsize];

	for (unsigned int r = 0; r < sqr; ++r)
		for (unsigned int c = 0; c < sqr; ++c)
		{
			verts[r * sqr + c].position = glm::vec4((c / (sqr - 1.f) - .5f) * dim, 0,
				(r / (sqr - 1.f) - .5f) * dim, 1);
			verts[r * sqr + c].texcoord = glm::vec2(c / (sqr - 1.f), r / (sqr - 1.f));
			verts[r * sqr + c].normal = glm::vec4(0, 1, 0, 0);
		}

	unsigned int index = 0;

	for (unsigned int r = 0; r < sqr - 1; ++r)
		for (unsigned int c = 0; c < sqr - 1; ++c)
		{
			tris[index++] = r * sqr + c;
			tris[index++] = (r + 1) * sqr + c;
			tris[index++] = (r + 1) *sqr + (c + 1);

			tris[index++] = r * sqr + c;
			tris[index++] = (r + 1) *sqr + (c + 1);
			tris[index++] = r *sqr + (c + 1);
		}
	geo = makeGeometry(verts, vsize, tris, tsize);

	delete[] tris;
	delete[] verts;
	return geo;

}
