#version 330 core

// Input
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

// Output
out VS_OUT {
        vec2 uv;
        vec3 position;
        vec3 tg_position;
        vec3 tg_cam_position;
        vec3 tg_light_position;
} vs_out;

// Transform matrices
uniform mat4 normal_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

// Scenario aspects
uniform vec3 cam_position;
uniform vec3 light_position;

void main()
{
        gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0);
        vs_out.position = vec3(model_matrix) * position;
        vs_out.uv = uv;

        vec3 T = normalize(vec3(normal_matrix) * tangent);
        vec3 B = normalize(vec3(normal_matrix) * bitangent);
        vec3 N = normalize(vec3(normal_matrix) * normal);
        mat3 TBN = transpose(mat3(T, B, N));

        vs_out.tg_position = TBN * (vec3(model_matrix) * position);
        vs_out.tg_cam_position = TBN * cam_position;
        vs_out.tg_light_position = TBN * light_position;
}
