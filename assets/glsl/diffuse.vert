#version 330 core
// profile: gp5vp

// Input
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvcoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

// Output
out VS_OUT {
        vec3 normal;
        vec2 uvcoord;
        vec3 position;
} vs_out;

// Transform matrices
uniform mat4 normal_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

void main()
{
        vec3 l_normal = normalize(vec3(normal_matrix * vec4(normal, 0.0)));
        vec3 l_position = vec3(model_matrix * vec4(position, 1.0));

        vs_out.normal = l_normal;
        vs_out.uvcoord = uvcoord;
        vs_out.position = l_position;

        gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0);
}
