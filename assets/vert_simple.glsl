#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

out vec2 vs_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);
	vs_texcoord = vec2(texcoord.x, 1.0f - texcoord.y);
}