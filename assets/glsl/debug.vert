#version 330 core
// profile: gp5vp

// Input
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_normal;
layout (location = 2) in vec2 vert_uvcoord;
layout (location = 3) in vec3 vert_tangent;
layout (location = 4) in vec3 vert_bitangent;

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

// Scenario aspects
uniform vec3 light_pos;
uniform vec3 viewer_pos;

void main()
{
        vec3 N = normalize(vec3(normal_matrix * vec4(vert_normal, 0.0)));
        vec3 T = normalize(vec3(normal_matrix * vec4(vert_tangent, 0.0)));
        vec3 B = normalize(vec3(normal_matrix * vec4(vert_bitangent, 0.0)));
        T = normalize(T - dot(T, N) * N);
        //vec3 B = cross(T, N);

        if (dot(cross(N, T), B) < 0.0) {
                T =  T * -1.0;
        }

        mat3 TBN = transpose(mat3(T, B, N));

        vs_out.uvcoord = vert_uvcoord;
        vs_out.light_dir = TBN * normalize(light_pos - vert_pos);
        //vs_out.viewer_dir = TBN * normalize(viewer_pos - vert_pos);
        vs_out.viewer_dir = normalize(viewer_pos - vert_pos);

        gl_Position = proj_matrix * view_matrix * model_matrix * vec4(vert_pos, 1.0);
}
