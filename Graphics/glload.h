#pragma once
#include "globjects.h"

Geometry loadObj(const char *path);


Texture loadTexture(const char *path);

Shader loadShader(const char *vpath, const char *fpath, bool depth = true, bool add = false, bool face = true);

