#include "so_textured_quad.h"

SoTexturedQuad::SoTexturedQuad()
{

}

void SoTexturedQuad::init(std::string textureName)
{
	_vsh.load_and_compile(GL_VERTEX_SHADER, "../vsh_ground.glsl");
	_fsh.load_and_compile(GL_FRAGMENT_SHADER, "../fsh_ground.glsl");
	_program.init_and_link(_vsh, _fsh);

	gen_vertex_arrays(1);
	gen_buffers(3);

	_program.uniform_locations(1);
	_program.uniform_location(0, "vTransf");
	_program.uniform_location(1, "vProj");
	_program.uniform_location(2, "vTexture");

	GsImage I;
	if (!I.load(textureName.c_str())) 
		std::cout << "COULD NOT LOAD IMAGE!\n";   
	
	glGenTextures(1, &textureHandle); 
	glBindTexture ( GL_TEXTURE_2D, textureHandle );   
	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, I.w(), I.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I.data() );
	glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glGenerateMipmap ( GL_TEXTURE_2D );
	glBindTexture ( GL_TEXTURE_2D, 0 );   
	
	glBindVertexArray ( 0 );
	I.init(0,0); // free image from CPU
}

void SoTexturedQuad::build(float tiling, float scale)
{
	struct vertex
	{
		GsVec position;
		GsVec2 texcoord;
	};

	struct vertex vertices[] =
	{
		{ GsVec(-0.5f, -0.5f, -0.5f) * scale, GsVec2(0,0) },
		{ GsVec(-0.5f, -0.5f, 0.5f) * scale, GsVec2(0,tiling) },
		{ GsVec(0.5f, -0.5f, -0.5f) * scale, GsVec2(tiling,0) },

		{ GsVec(0.5f, -0.5f, 0.5f) * scale, GsVec2(tiling,tiling) },
		{ GsVec(0.5f, -0.5f, -0.5f) * scale, GsVec2(tiling,0) },
		{ GsVec(-0.5f, -0.5f, 0.5f) * scale, GsVec2(0,tiling) },

		{ GsVec(-0.5f, 0.5f, -0.5f) * scale, GsVec2(0,0) },
		{ GsVec(-0.5f, 0.5f, 0.5f) * scale, GsVec2(0,tiling) },
		{ GsVec(0.5f, 0.5f, -0.5f) * scale, GsVec2(tiling,0) },

		{ GsVec(0.5f, 0.5f, 0.5f) * scale, GsVec2(tiling,tiling) },
		{ GsVec(0.5f, 0.5f, -0.5f) * scale, GsVec2(tiling,0) },
		{ GsVec(-0.5f, 0.5f, 0.5f) * scale, GsVec2(0,tiling) },


		{ GsVec(0.5f, -0.5f, -0.5f) * scale, GsVec2(0,0) },
		{ GsVec(0.5f, -0.5f, 0.5f) * scale, GsVec2(0,tiling) },
		{ GsVec(0.5f, 0.5f, -0.5f) * scale, GsVec2(tiling,0) },

		{ GsVec(0.5f, 0.5f, 0.5f) * scale, GsVec2(tiling,tiling) },
		{ GsVec(0.5f, 0.5f, -0.5f) * scale, GsVec2(tiling,0) },
		{ GsVec(0.5f, -0.5f, 0.5f) * scale, GsVec2(0,tiling) },

		{ GsVec(-0.5f, -0.5f, -0.5f) * scale, GsVec2(0,0) },
		{ GsVec(-0.5f, -0.5f, 0.5f) * scale, GsVec2(0,tiling) },
		{ GsVec(-0.5f, 0.5f, -0.5f) * scale, GsVec2(tiling,0) },

		{ GsVec(-0.5f, 0.5f, 0.5f) * scale, GsVec2(tiling,tiling) },
		{ GsVec(-0.5f, 0.5f, -0.5f) * scale, GsVec2(tiling,0) },
		{ GsVec(-0.5f, -0.5f, 0.5f) * scale, GsVec2(0,tiling) },



		{ GsVec(-0.5f, -0.5f, -0.5f) * scale, GsVec2(0,0) },
		{ GsVec(-0.5f, 0.5f, -0.5f) * scale, GsVec2(0,tiling) },
		{ GsVec(0.5f, -0.5f, -0.5f) * scale, GsVec2(tiling,0) },

		{ GsVec(0.5f, 0.5f, -0.5f) * scale, GsVec2(tiling,tiling) },
		{ GsVec(0.5f, -0.5f, -0.5f) * scale, GsVec2(tiling,0) },
		{ GsVec(-0.5f, 0.5f, -0.5f) * scale, GsVec2(0,tiling) },

		{ GsVec(-0.5f, -0.5f, 0.5f) * scale, GsVec2(0,0) },
		{ GsVec(-0.5f, 0.5f, 0.5f) * scale, GsVec2(0,tiling) },
		{ GsVec(0.5f, -0.5f, 0.5f) * scale, GsVec2(tiling,0) },

		{ GsVec(0.5f, 0.5f, 0.5f) * scale, GsVec2(tiling,tiling) },
		{ GsVec(0.5f, -0.5f, 0.5f) * scale, GsVec2(tiling,0) },
		{ GsVec(-0.5f, 0.5f, 0.5f) * scale, GsVec2(0,tiling) },
	};

	glBindVertexArray(va[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (const GLvoid *)&vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (const GLvoid *)offsetof(struct vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (const GLvoid *)offsetof(struct vertex, texcoord));

	glBindVertexArray(0);
}

void SoTexturedQuad::draw(GsMat& tr, GsMat& pr)
{
	glUseProgram(_program.id);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glUniformMatrix4fv(_program.uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(_program.uniloc[1], 1, GL_FALSE, pr.e);
	glUniform1i(_program.uniloc[2], 0);

	glBindVertexArray(va[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}