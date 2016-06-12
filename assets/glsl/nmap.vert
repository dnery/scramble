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
        vec2 uvcoord;
        vec3 light_dir;
        vec3 viewer_dir;
} vs_out;

// Transform matrices
uniform mat4 normal_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

// Viewer & caster
uniform vec3 light_pos;
uniform vec3 viewer_pos;

void main()
{
        vec3 N = normalize(vec3(normal_matrix * vec4(normal, 0.0)));
        vec3 T = normalize(vec3(normal_matrix * vec4(tangent, 0.0)));
        vec3 B = normalize(vec3(normal_matrix * vec4(bitangent, 0.0)));

        if (dot(cross(N, T), B) < 0.0) {
                T = T * -1.0;
        }

        mat3 TBN = transpose(mat3(T, B, N));

        vec3 l_position = vec3(model_matrix * vec4(position, 1.0));

        vs_out.uvcoord = uvcoord;
        vs_out.light_dir = TBN * normalize(light_pos - l_position);
        vs_out.viewer_dir = TBN * normalize(viewer_pos - l_position);

        gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0);
}
