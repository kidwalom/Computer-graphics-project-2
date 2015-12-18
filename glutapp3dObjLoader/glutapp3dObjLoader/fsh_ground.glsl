# version 400

uniform sampler2D vTexture;

in vec2 Tex;
out vec4 fColor;

void main() 
{
	fColor = texture(vTexture, Tex);
} 
