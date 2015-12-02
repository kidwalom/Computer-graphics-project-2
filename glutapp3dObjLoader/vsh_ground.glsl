# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;

uniform mat4 vTransf;
uniform mat4 vProj;

out vec2 Tex;

void main ()
{
	Tex = vTex;
	gl_Position = vec4(vPos, 1.0) * vTransf * vProj;
}

