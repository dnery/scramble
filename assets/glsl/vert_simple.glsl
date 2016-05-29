#version 330 core
// profile: gp5vp

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

out vec2 vs_texcoord;
out vec3 vs_worldpos;
out vec3 vs_normal;

uniform mat4 normat;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
        gl_Position = proj * view * model * vec4(position, 1.0f);

        vs_texcoord = vec2(texcoord.x, 1.0f - texcoord.y);

        vs_worldpos = vec3(model * vec4(position, 1.0f));

        vs_normal = normalize(mat3(normat) * normal);
}
