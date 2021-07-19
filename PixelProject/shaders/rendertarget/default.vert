#version 410
layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 TexCoord;

out vec2 fTexCoord;

void main()
{
	fTexCoord = TexCoord;
	gl_Position = Position;
}