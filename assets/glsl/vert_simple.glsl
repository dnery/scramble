#version 330 core
// profile: gp5vp

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out vec2 vs_texcoord;
out vec3 vs_worldpos;
out vec3 vs_normal;

uniform mat4 normal_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

void main()
{
        gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0f);

        vs_texcoord = vec2(texcoord.x, 1.0f - texcoord.y);

        vs_worldpos = vec3(model_matrix * vec4(position, 1.0f));

        vs_normal = normalize(mat3(normal_matrix) * normal);
}
