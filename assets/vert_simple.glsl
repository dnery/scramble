#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 pixcolor;
layout (location = 2) in vec2 texcoord;

out vec3 vs_position;
out vec3 vs_pixcolor;
out vec2 vs_texcoord;

void main()
{
	gl_Position = vec4(position, 1.0f);
	vs_position = position;
	vs_pixcolor = pixcolor;
	vs_texcoord = vec2(texcoord.x, 1.0f - texcoord.y);
}
