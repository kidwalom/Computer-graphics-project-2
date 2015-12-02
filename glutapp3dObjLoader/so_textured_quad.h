#pragma once

#include "gsim/gs.h"
#include "ogl_tools.h"
#include "gsim/gs_image.h"

class SoTexturedQuad : GlObjects
{
	GlShader _vsh, _fsh;
	GlProgram _program;

	GLuint textureHandle;

public:
	SoTexturedQuad();

	void init(std::string textureName);
	void build(float tiling, float scale);
	void draw(GsMat& tr, GsMat& pr);
};
